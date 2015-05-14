var client_id="CKj4vKwqaYr"
var secret="q6eEiPxV2smpJrsDCS4H327rdhp39oTZ"

var Hackpad = require('hackpad');
var client = new Hackpad(client_id, secret);

client.list(function(err, resp){
}    console.log(err);
    console.log(resp);    
});
/*client.export("Sg7Iq7GM9Ju",function(err, resp) {
    console.log("hey!");
    console.log(err);
    console.log(resp);
});
*/