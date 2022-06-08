from django.contrib import admin
from django.urls import path
from pbl.views import *

urlpatterns = [
    path('admin/', admin.site.urls),
    path('', index, name='index'),

    path("register/", register_request, name="register"),
    path("login/", login_request, name="login"),
    path("logout/", logout_request, name="logout"),
    path("attach/", attach_device, name="attach"),
    path('device/<int:device_id>/', device_view, name='device'),
    path('device/<int:device_id>/<str:temperature_delta>', device_control, name='device_control'),
    path('profile/', profile_view, name='profile'),
    path('device/<int:device_id>/delete/', device_delete, name='device_delete'),
]
