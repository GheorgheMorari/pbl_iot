from django import forms
from django.contrib.auth.forms import UserCreationForm

# Create your forms here.
from pbl.models import SiteUser


class NewUserForm(UserCreationForm):
    class Meta:
        model = SiteUser
        fields = ("username", "password1", "password2")

    def save(self, commit=True):
        user = super(NewUserForm, self).save(commit=False)
        if commit:
            user.save()
        return user
