from django.shortcuts import render
from .models import Devices

import logging
logger = logging.getLogger(__name__)

# Create your views here.
def add_device(request):
	device_dict = {
		"device_added": False,
		"device": {
			"name": "",
			"type": "",
		},
	}
	if request.method == 'POST':
		device_name = request.POST['device_name']
		device_type = request.POST['device_type']

		new_device = Devices(device_name=device_name, device_type=device_type)
		new_device.save()
		device_dict["device_added"] = True
		device_dict["device"]["name"] = device_name
		device_dict["device"]["type"] = device_type

	return render(request, 'add_device.html', device_dict)

def device_list(request):
	all_devices = Devices.objects.all().values()
	device_dict = {
		"all_devices": all_devices, 
	}
	return render(request, 'devices.html', device_dict)

def device_details(request, id):
	device_details = Devices.objects.get(id=id)
	device_dict = {
		"device_details": device_details, 
	}
	return render(request, 'details.html', device_dict)

def remove_device(request):
	all_devices = Devices.objects.all().values()
	device_dict = {
		"all_devices": all_devices, 
		"remove_device": False
	}
	if request.method == 'POST':
		logger.debug(request.POST)
		device_id = request.POST['device_id']
		device_to_remove = Devices.objects.filter(id=device_id).delete()
		device_dict["remove_device"] = True

	return render(request, 'remove_device.html', device_dict)

def update_device(request):
	all_devices = Devices.objects.all().values()
	device_dict = {
		"all_devices": all_devices,
		"updated_name": False,
		"updated_type": False, 
	}
  	# TODO: Add code to analyse the user input, html validation is not enough.
	logger.debug("1")
	if request.method == 'POST':
		logger.debug("2")
		logger.debug(request.POST)
		device_id = request.POST['device_id']
		new_device_name = request.POST['new_device_name']
		new_device_type = request.POST['new_device_type']

		device_to_update = Devices.objects.filter(id=device_id)
		current_name = device_to_update.values()[0]["device_name"]
		current_type = device_to_update.values()[0]["device_type"]

		if current_name != new_device_name and current_type != new_device_type:
			device_dict["updated_name"] = True
			device_dict["updated_type"] = True
			device_to_update.update(device_name=new_device_name, device_type=new_device_type)
		elif current_name != new_device_name:
			device_dict["updated_name"] = True
			device_to_update.update(device_name=new_device_name)
		elif current_type != new_device_type:
			device_dict["updated_type"] = True
			device_to_update.update(device_type=new_device_type)
		else:
			device_dict["updated_name"] = False
			device_dict["updated_type"] = False
		logger.debug("3")

	return render(request, 'update_device.html', device_dict)
	
