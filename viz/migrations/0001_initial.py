# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('cube', '__first__'),
    ]

    operations = [
        migrations.CreateModel(
            name='Binary',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('file', models.FileField(upload_to=b'binaries/%Y/%m/%d')),
                ('created', models.DateTimeField(auto_now=True)),
            ],
        ),
        migrations.CreateModel(
            name='Photo',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('file', models.ImageField(null=True, upload_to=b'photos/%Y/%m/%d', blank=True)),
            ],
        ),
        migrations.CreateModel(
            name='SourceCode',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('code', models.TextField(default=b'')),
                ('created', models.DateTimeField(auto_now_add=True)),
                ('updated', models.DateTimeField(auto_now=True)),
            ],
        ),
        migrations.CreateModel(
            name='Viz',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('name', models.TextField()),
                ('description', models.TextField()),
                ('created', models.DateTimeField(auto_now_add=True)),
                ('sourceURL', models.TextField(blank=True)),
                ('tags', models.TextField(blank=True)),
                ('views', models.IntegerField(default=0)),
                ('vizType', models.TextField(default=b'L3D')),
                ('videoURL', models.TextField(blank=True)),
                ('binaryPath', models.TextField(blank=True)),
                ('interactive', models.BooleanField(default=False)),
                ('published', models.BooleanField(default=False)),
                ('pageViews', models.IntegerField(default=0)),
                ('creator', models.ForeignKey(to='cube.CubeUser')),
                ('parent', models.ForeignKey(default=None, blank=True, to='viz.Viz', null=True)),
            ],
        ),
        migrations.AddField(
            model_name='sourcecode',
            name='viz',
            field=models.ForeignKey(to='viz.Viz'),
        ),
        migrations.AddField(
            model_name='photo',
            name='viz',
            field=models.ForeignKey(to='viz.Viz'),
        ),
        migrations.AddField(
            model_name='binary',
            name='viz',
            field=models.ForeignKey(to='viz.Viz'),
        ),
    ]
