# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('cube', '__first__'),
        ('viz', '0001_initial'),
    ]

    operations = [
        migrations.CreateModel(
            name='Rating',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('date', models.DateTimeField(auto_now_add=True)),
                ('reviewer', models.ForeignKey(to='cube.CubeUser')),
            ],
        ),
        migrations.AddField(
            model_name='viz',
            name='averageRating',
            field=models.FloatField(default=0),
            preserve_default=False,
        ),
        migrations.AddField(
            model_name='viz',
            name='numberOfRatings',
            field=models.IntegerField(default=0),
            preserve_default=False,
        ),
        migrations.AddField(
            model_name='rating',
            name='viz',
            field=models.ForeignKey(to='viz.Viz'),
        ),
    ]
