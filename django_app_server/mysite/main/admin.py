from django.contrib import admin
from .models import Tutorial, LightStatus
# Register your models here.

class TutorialAdmin(admin.ModelAdmin):
	fields = ["tutorial_title",
	          "tutorial_published",
			   "tutorial_content"]

class LightStatusAdmin(admin.ModelAdmin):
	fields = ["light_name", "status"]

admin.site.register(Tutorial, TutorialAdmin)
admin.site.register(LightStatus, LightStatusAdmin)

