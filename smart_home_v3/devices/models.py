from django.db import models

# Create your models here.
class Devices(models.Model):
  DEVICES = {
      "Light": "Light",
      "Lock": "Lock",
      "Camera": "Camera",
  }
  device_name = models.CharField(max_length=20)
  device_type = models.CharField(max_length=20, choices=DEVICES)