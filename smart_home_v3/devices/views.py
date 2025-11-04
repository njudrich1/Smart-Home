from django.shortcuts import render
from .models import Devices

import logging
logger = logging.getLogger(__name__)

# Create your views here.
def add_device(request):
	if request.method == 'POST':
		device_name = request.POST['device_name']
		device_type = request.POST['device_type']

		new_device = Devices(device_name=device_name, device_type=device_type)
		new_device.save()

	return render(request, 'add_device.html', {})

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
	}
	if request.method == 'POST':
		device_name = request.POST['device_name']
		device_to_remove = Devices.objects.filter(device_name=device_name).delete()

	return render(request, 'remove_device.html', device_dict)

def update_device(request):
	all_devices = Devices.objects.all().values()
	device_dict = {
		"all_devices": all_devices, 
	}

	logger.debug("1")
	if request.method == 'POST':
		logger.debug("2")
		logger.debug(request.POST)
		device_name = request.POST['device_name']
		device_to_update = Devices.objects.filter(device_name=device_name).update(device_name=request.POST['new_device_name'], device_type=request.POST['new_device_type'])
		logger.debug("3")

	return render(request, 'update_device.html', device_dict)
	
