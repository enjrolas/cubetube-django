# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('viz', '0001_initial'),
    ]

    operations = [
        migrations.AddField(
            model_name='viz',
            name='interactive',
            field=models.BooleanField(default=False),
        ),
        migrations.AddField(
            model_name='viz',
            name='videoURL',
            field=models.TextField(blank=True),
        ),
    ]
