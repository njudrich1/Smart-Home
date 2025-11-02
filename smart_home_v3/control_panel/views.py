from django.shortcuts import render
from django.http import HttpResponse

# Create your views here.
def light_control(request):
    return render(request, 'base_panel_template.html', {})
