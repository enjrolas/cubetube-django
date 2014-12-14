from django.db import models
from cube.models import CubeUser

class Viz(models.Model):
    name=models.TextField()
    description=models.TextField()
    creator=models.ForeignKey(CubeUser)
    created=models.DateTimeField()
    binaryURL=models.TextField()
    sourceURL=models.TextField()
    def __str__(self):
        return "%s, created by %s on %s" %(self.name, self.creator, self.created)

class Photo(models.Model):
    viz=models.ForeignKey(Viz)
    photoURL=models.TextField()
    title=models.TextField(null=True)
    description=models.TextField(null=True)
    mainImage=models.BooleanField()
    def __str__(self):
        return "%s:  %s - %s" %(self.viz.name, self.title, self.photoURL)
