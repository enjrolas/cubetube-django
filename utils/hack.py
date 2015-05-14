import os
from hackpad_api.hackpad import Hackpad

hackpad = Hackpad(consumer_key="CKj4vKwqaYr", consumer_secret = "q6eEiPxV2smpJrsDCS4H327rdhp39oTZ")

print(hackpad.do_api_request('pads/all', 'GET'))
print(hackpad.get_pad_content("mlZvEsJykI5"))
