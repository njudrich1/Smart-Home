from django.shortcuts import render
from django.http import HttpResponse
from .models import Tutorial, LightStatus
import logging

logger = logging.getLogger(__name__)

# Create your views here.
def homepage(request):
	logger.debug("1")
	logger.debug(request)
	logger.debug(f"Request Method: {request.method}")
	logger.debug(f"Request POST: {request.POST}")
	if request.method == 'POST':
		logger.debug("2")
		light_status = request.POST['light_state']
		# Save status to DB
		logger.debug("3")
		# Send request serially to MCU
		logger.debug("4")

	return render(request=request,
				  template_name='main/home.html',
				  context={"tutorials": Tutorial.objects.all})


