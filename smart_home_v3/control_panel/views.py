from django.shortcuts import render
from django.http import HttpResponse
from .models import Devices

import logging
logger = logging.getLogger(__name__)

# Create your views here.
def light_controller(request):
    all_devices = Devices.objects.all().values()
    device_dict = {
        "all_devices": all_devices, 
    }
    logger.debug("1")
    logger.debug(request)
    light = {
        "light_state": "off",
    }
    logger.debug(f"Request Method: {request.method}")
    logger.debug(f"Request POST: {request.POST}")
        # if request.method == 'POST':
    #   logger.debug("2")
    #   light_status = request.POST['light_state']
    #   light = {
    #   "light_state": light_status,
    #   }
    #   # Save status to DB
    #   logger.debug("3")
    #   # Send request serially to MCU
    #   if not ser.is_open:
    #       ser.open()
    #   if light_status == 'on':
    #       data = 'o'
    #   if light_status == 'off':
    #       data = 'f'
    #   ser.write(data.encode('utf-8'))
        
    #   logger.debug("4")

    return render(request, 'light_controller.html', device_dict)
