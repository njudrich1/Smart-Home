from django.shortcuts import render, redirect
from django.http import HttpResponse
from .models import Devices

import serial

import logging
logger = logging.getLogger(__name__)

cache = {
    "controller": "",
    "device": {
        "name": "",
        "type": "",
    },
    "device_selected": False,
}

# Create your views here.
def controller_selector(request):
    all_devices = Devices.objects.all().values()
    device_dict = {
        "all_devices": all_devices,
        "controller": {},
        "device" : {
            "name": "", 
            "type": "", 
            "state": "",
        },
        "device_selected": False,
        "confirm_device_state": False
    }
    if request.method == 'POST':
        logger.debug(request.POST)
        if 'device_id' in request.POST:
            device_id = request.POST['device_id']
            device_selected = _identify_selected_device(device_id)
            controller = _identify_device_controller(device_selected)
            if controller:
                device_dict['controller'] = controller
                device_dict['device']['name'] = device_selected.device_name
                device_dict['device']['type'] = device_selected.device_type
                device_dict['device_selected'] = True
                cache['device_selected'] = device_dict['device_selected']
                cache["controller"] = controller
                cache["device"]["name"] = device_selected.device_name
                cache["device"]["type"] = device_selected.device_type
                logger.debug("1")
        if 'device_state' in request.POST:
            # TODO Retrieve device status from DB and update accordingly.
            device_state = request.POST['device_state']
            device_dict['device']['state'] = device_state 
            device_dict['controller'] = cache["controller"]
            device_dict['device']['name'] = cache["device"]["name"]
            device_dict['device']['type'] = cache["device"]["type"]
            device_dict['device_selected'] = cache['device_selected']
            device_dict['confirm_device_state'] = True
            process_data(device_dict)
        logger.debug(f"device dict: {device_dict}")

    return render(request, 'controller_selector.html', device_dict)

def process_data(device_data):
    ser = serial.Serial('COM5', 9600)
    if not ser.is_open:
        ser.open()
    
    if device_data['controller'] == 'light_controller':
        if device_data['device']['state'] == "on":
            #data = '1'
            #frame = "s11&r"
            logger.debug("encode ON data and send")
        if device_data['device']['state'] == "off":
            logger.debug("encode OFF data and send")
        #msg_length = str(len(data))
        #frame = 's'+ msg_length + data + '&r'
        #ser.write(frame.encode('utf-8))
        #RX = ser.read(3)  # DBG
        #logger.debug(f"Recieved Data: {RX}") # DBG

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