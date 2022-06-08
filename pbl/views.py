import random
import string

from django.contrib import messages
from django.contrib.auth import authenticate, logout, login
from django.contrib.auth.forms import AuthenticationForm
from django.http import Http404, HttpResponseBadRequest, HttpResponse
from django.shortcuts import render, redirect

from mqtt import mqtt_manager
from pbl.forms import NewUserForm
from pbl.models import SiteUser, Device, mqtt_callback


# The main view where the users can see all the post objects
def index(request):
    user = None
    private_posts = None
    if request.user.is_authenticated:
        user = SiteUser.objects.get(id=request.user.id)

    context = {
        # 'posts': posts,
        'user': user,
        'private_posts': private_posts,
    }
    return render(request, 'index.html', context)


def login_request(request):
    # Check if the user is not authenticated
    if request.user.is_authenticated:
        return redirect('index')
    message = ''
    if request.method == "GET":
        form = AuthenticationForm(request, data=request.GET)
        if form.is_valid():
            username = form.cleaned_data.get('username')
            password = form.cleaned_data.get('password')
            user = authenticate(username=username, password=password)
            if user is not None:
                login(request, user)
                messages.info(request, f"You are now logged in as {username}.")
                return redirect("index")
            else:
                message = "Invalid username or password."
                messages.error(request, "Invalid username or password.")
        else:
            message = "Invalid username or password."
            messages.error(request, "Invalid username or password.")
    form = AuthenticationForm()
    return render(request=request, template_name="login.html", context={"login_form": form, "message": message})


def logout_request(request):
    logout(request)
    messages.info(request, "Logged out successfully!")
    return redirect("index")


def register_request(request):
    if request.method == "POST":
        form = NewUserForm(request.POST)
        if form.is_valid():
            user = form.save()
            login(request, user)
            user_obj = SiteUser.objects.get(id=user.id)
            user_obj.subscriptions.add(user_obj)
            messages.success(request, "Registration successful.")
            return redirect("index")
        messages.error(request, "Unsuccessful registration. Invalid information.")
    form = NewUserForm()
    return render(request=request, template_name="register.html", context={"register_form": form})


def attach_device(request):
    # Extract parameters from request
    if 'username' not in request.GET or 'password' not in request.GET:
        return HttpResponseBadRequest()
    username = request.GET['username']
    password = request.GET['password']
    user = authenticate(username=username, password=password)
    if user is not None:
        login(request, user)
        # Create unique device id alpha-numeric
        device_name = ''.join(
            random.choice(string.ascii_uppercase + string.ascii_lowercase + string.digits) for _ in range(16))
        # Create device object
        device = Device.objects.create(name=device_name)
        # Add device to user
        user_obj = SiteUser.objects.get(id=user.id)
        user_obj.devices.add(device)
        mqtt_manager.subscribe(device_name + '/data', mqtt_callback)
        return HttpResponse(device_name)
    else:
        return Http404()


def profile_view(request):
    if request.user.is_authenticated:
        user = SiteUser.objects.get(id=request.user.id)
        return render(request, 'profile.html',
                      {'user': user,
                       'devices': user.devices.all()})

    return redirect("index")


def device_view(request, device_id):
    if request.user.is_authenticated:
        user = SiteUser.objects.get(id=request.user.id)
        device = Device.objects.get(id=device_id)
        return render(request, 'device.html',
                      {'user': user,
                       'device': device})

    return redirect("index")


def device_control(request, device_id, temperature_delta):
    print("BRUH " + temperature_delta)
    if request.user.is_authenticated:
        device = Device.objects.get(id=device_id)
        mqtt_manager.publish(device.name + '/control', temperature_delta)
        return redirect("device", device_id=device_id)

    return redirect("index")


def device_delete(request, device_id):
    if request.user.is_authenticated:
        device = Device.objects.get(id=device_id)
        device.delete()
        mqtt_manager.unsubscribe(device.name + '/data')
        return redirect("profile")

    return redirect("index")
