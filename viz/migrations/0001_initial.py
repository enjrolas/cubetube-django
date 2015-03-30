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
            options={
            },
            bases=(models.Model,),
        ),
        migrations.CreateModel(
            name='Photo',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('file', models.ImageField(null=True, upload_to=b'photos/%Y/%m/%d', blank=True)),
            ],
            options={
            },
            bases=(models.Model,),
        ),
        migrations.CreateModel(
            name='SourceCode',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('code', models.TextField(default=b'')),
                ('created', models.DateTimeField(auto_now_add=True)),
                ('updated', models.DateTimeField(auto_now=True)),
            ],
            options={
            },
            bases=(models.Model,),
        ),
        migrations.CreateModel(
            name='Viz',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('name', models.TextField()),
                ('tagline', models.TextField()),
                ('description', models.TextField()),
                ('created', models.DateTimeField(auto_now_add=True)),
                ('sourceURL', models.TextField(default=b'')),
                ('tags', models.TextField(default=b'')),
                ('views', models.IntegerField(default=0)),
                ('vizType', models.TextField(default=b'spark')),
                ('pageViews', models.IntegerField(default=0)),
                ('creator', models.ForeignKey(to='cube.CubeUser')),
                ('parent', models.ForeignKey(default=None, blank=True, to='viz.Viz', null=True)),
            ],
            options={
            },
            bases=(models.Model,),
        ),
        migrations.AddField(
            model_name='sourcecode',
            name='viz',
            field=models.ForeignKey(to='viz.Viz'),
            preserve_default=True,
        ),
        migrations.AddField(
            model_name='photo',
            name='viz',
            field=models.ForeignKey(to='viz.Viz'),
            preserve_default=True,
        ),
        migrations.AddField(
            model_name='binary',
            name='viz',
            field=models.ForeignKey(to='viz.Viz'),
            preserve_default=True,
        ),
    ]
