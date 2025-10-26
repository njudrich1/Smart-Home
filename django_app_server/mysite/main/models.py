from django.db import models

# Create your models here.
class Tutorial(models.Model):
	tutorial_title = models.CharField(max_length=200)
	tutorial_content = models.TextField()
	tutorial_published = models.DateTimeField("date published")

	def __str__(self):
		return self.tutorial_title

class LightStatus(models.Model):
	light_name = models.CharField(max_length=200)
	status = models.CharField(max_length=3)

	def __str__(self):
		return self.light_name
