from django.db import models
from cube.models import CubeUser

class Viz(models.Model):
    name=models.TextField()
    tagline=models.TextField()
    description=models.TextField()
    creator=models.ForeignKey(CubeUser)
    created=models.DateTimeField(auto_now_add=True)
    sourceURL=models.TextField(default="")
    tags=models.TextField(default="")
    views=models.IntegerField(default=0)
    vizType=models.TextField(default="spark")
    pageViews=models.IntegerField(default=0)
    parent=models.ForeignKey('self', null=True, blank=True, default = None)
    def __str__(self):
        return "%s, created by %s on %s" %(self.name, self.creator, self.created)

class SourceCode(models.Model):
    viz=models.ForeignKey(Viz)
    code=models.TextField(default="")
    created=models.DateTimeField(auto_now_add=True)
    updated=models.DateTimeField(auto_now=True)

class Photo(models.Model):
    viz=models.ForeignKey(Viz)
    file=models.ImageField(upload_to='photos/%Y/%m/%d', null=True, blank=True)
    def __str__(self):
        return "%s:  %s" %(self.viz.name, self.file)

class Binary(models.Model):
    file=models.FileField(upload_to='binaries/%Y/%m/%d')
    viz=models.ForeignKey(Viz)
    created=models.DateTimeField(auto_now=True)
    def __str__(self):
        return "%s binary " % (self.file)
