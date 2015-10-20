# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('viz', '0002_auto_20150817_1350'),
    ]

    operations = [
        migrations.AlterField(
            model_name='viz',
            name='averageRating',
            field=models.FloatField(default=0),
        ),
        migrations.AlterField(
            model_name='viz',
            name='numberOfRatings',
            field=models.IntegerField(default=0),
        ),
    ]
