# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('viz', '0003_viz_published'),
    ]

    operations = [
        migrations.AddField(
            model_name='viz',
            name='binaryPath',
            field=models.TextField(blank=True),
        ),
    ]
