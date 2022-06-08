from django.contrib import admin

# Register posts and users
from django.contrib.auth.models import User

from .models import *

# admin.site.unregister(User)

admin.site.register(SiteUser)
