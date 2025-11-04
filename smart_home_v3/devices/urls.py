"""
URL configuration for smart_home_v3 project.

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/5.2/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.contrib import admin
from django.urls import path
from . import views

urlpatterns = [
    path('add_device/', views.add_device, name="add_device"),
    path('devices/', views.device_list, name="device_list"),
    path('devices/details/<int:id>', views.device_details, name="device_details"),
    path('remove_device/', views.remove_device, name="remove_device"),
    path('update_device/', views.update_device, name="update_device"),
]
