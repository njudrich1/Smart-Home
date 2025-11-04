from django.shortcuts import render, redirect
from django.http import HttpResponse
from .models import Devices

import logging
logger = logging.getLogger(__name__)

# Create your views here.
def controller_selector(request):
    all_devices = Devices.objects.all().values()
    controller = {}
    selected_device = {"device_name": "", "device_type": ""}
    device_dict = {
        "all_devices": all_devices,
        "controller": controller,
        "selected_device" : selected_device,
    }
    logger.debug("1")
    logger.debug(f"device_dict: {device_dict}")
    logger.debug(request)
    logger.debug(f"Request Method: {request.method}")
    logger.debug(f"Request POST: {request.POST}")
    if request.method == 'POST':
        device_id = request.POST['device_id']
        device_selected = _identify_selected_device(device_id)
        logger.debug("2")
        logger.debug(f"Device Name: {device_selected.device_name}")
        logger.debug(f"Device Type: {device_selected.device_type}")
        controller = _identify_device_controller(device_selected)
        if controller:
            device_dict['controller'] = controller
            device_dict['selected_device']['device_name'] = device_selected.device_name
            device_dict['selected_device']['device_type'] = device_selected.device_type 
            logger.debug(f"device dict: {device_dict}")

    return render(request, 'controller_selector.html', device_dict)

# def light_controller(request):
#     all_devices = Devices.objects.all().values()
#     device_dict = {
#         "all_devices": all_devices, 
#     }
#     logger.debug("1 - Light Controller")
#     logger.debug(f"device_dict: {device_dict}")
#     logger.debug(request)
#     light = {
#         "light_state": "off",
#     }
#     logger.debug(f"Request Method: {request.method}")
#     logger.debug(f"Request POST: {request.POST}")

#     if request.method == 'POST':
#         device_id = request.POST['device_id']
#         device_selected = _identify_selected_device(device_id)
#         selected_controller = _identify_device_controller(device_selected)
#         if selected_controller:
#             return redirect(selected_controller)

#         # if request.method == 'POST':
#     #   logger.debug("2")
#     #   light_status = request.POST['light_state']
#     #   light = {
#     #   "light_state": light_status,
#     #   }
#     #   # Save status to DB
#     #   logger.debug("3")
#     #   # Send request serially to MCU
#     #   if not ser.is_open:
#     #       ser.open()
#     #   if light_status == 'on':
#     #       data = 'o'
#     #   if light_status == 'off':
#     #       data = 'f'
#     #   ser.write(data.encode('utf-8'))
        
#     #   logger.debug("4")

#     return render(request, 'light_controller.html', device_dict)

def _identify_selected_device(device_id):
    logger.debug("BEGIN: _identify_selected_device")
    logger.debug(f"Device id: {device_id}")

    device_selected = Devices.objects.get(id=device_id)

    logger.debug(f"Device Selected: {device_selected}")
    logger.debug(f"Device Type: {device_selected.device_type}")
    logger.debug("END: _identify_selected_device")
    return device_selected

def _identify_device_controller(device):
    logger.debug("BEGIN: _identify_device_controller")

    if device.device_type == 'Light':
        device_controller = "light_controller"
        logger.debug("Light Controller")

    if device.device_type == 'Camera':
        device_controller = "camera_controller"
        logger.debug("Camera TBC")

    if device.device_type == 'Lock':
        device_controller = "lock_controller"
        logger.debug("Lock TBC") 

    logger.debug(f"Controller Selected: {device_controller}")
    logger.debug("END: _identify_device_controller")
    return device_controller