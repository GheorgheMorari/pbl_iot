from django.db import models
from django.contrib.auth.models import AbstractUser


class SiteUser(AbstractUser):
    devices = models.ManyToManyField('Device', related_name='users')

    def __str__(self):
        return self.username


class Device(models.Model):
    name = models.CharField(max_length=255, unique=True, db_index=True)
    last_update = models.CharField(max_length=255, default='')

    def __str__(self):
        return self.name


def mqtt_callback(client, userdata, message):
    device_name = message.topic.split('/')[0]
    print("Received message '" + str(message.payload.decode("utf-8")) + "' on topic '"
          + message.topic + "' with QoS " + str(message.qos))
    device = Device.objects.get(name=device_name)
    device.last_update = str(message.payload.decode("utf-8"))
    device.save()
