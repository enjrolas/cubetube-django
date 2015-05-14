from requests_oauthlib import OAuth1Session
hackpad = OAuth1Session('CKj4vKwqaYr',
                        client_secret='q6eEiPxV2smpJrsDCS4H327rdhp39oTZ')
#                        resource_owner_key='resource_owner_key',
#                        resource_owner_secret='resource_owner_secret')
url='https://hackpad.com/ep/pad/export/Sg7Iq7GM9Ju/latest?format=txt'
#url = 'https://hackpad.com/L3D-website-documentation-Sg7Iq7GM9Ju'
r = hackpad.get(url)
print(r.content)
