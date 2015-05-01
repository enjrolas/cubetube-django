var sparkCode;
var translatedCode="";
var canvasControl="";

//background(0);\ntranslate(width/2, height/2);\nrotateY(mouseX/50);\nrotateX(mouseY/50);
//we don't need this, now that I moved it to the library draw() function

var translation={};
translation["loop()"]="draw()";
translation["cube.show()"]="cube.draw()";
translation["Point"]="PVector";
translation["black)"]= "color(0x00, 0x00, 0x00))";
translation["grey)"]= "color(0x92, 0x95, 0x91))";
translation["yellow)"]= "color(0xff, 0xff, 0x14))";
translation["magenta)"]= "color(0xc2, 0x00, 0x78))";
translation["orange)"]= "color(0xf9, 0x73, 0x06))";
translation["teal)"]= "color(0x02, 0x93, 0x86))";
translation["red)"]= "color(0xe5, 0x00, 0x00))";
translation["brown)"]= "color(0x65, 0x37, 0x00))";
translation["pink)"]= "color(0xff, 0x81, 0xc0))";
translation["blue)"]= "color(0x03, 0x43, 0xdf))";
translation["green)"]= "color(0x15, 0xb0, 0x1a))";
translation["purple)"]= "color(0x7e, 0x1e, 0x9c))";
translation["white)"] = "color(0xff, 0xff, 0xff))";
translation["int "]="var ";
translation["unsigned int "]="var ";
translation["char "]="var ";
translation["unsigned char "]="var ";
translation["String "]="var ";
translation["float "]="var ";
translation["Color "]="color ";
translation["Color("]="color(";
translation["rand()"]="random(1000000)";

var parts;
var ignoreList=["include", "cube.begin"];

var flagList={};
flagList["Cube "]=0;
flagList["draw()"]=0;
flagList["PVector"]=0;
flagList["setup()"]=0;

function parseSparkCode( url, after ) {

    $.get(url, function(data) {

        sparkCode=data.split('\n');  //sparkCode is an array of lines in the data file
        jsCode=[];
        var cubeObject;
        var cubeObjectName;
        
        for( var i=0; i<sparkCode.length; i++ ) {
            $("#sparkCode").append(sparkCode[i]+"<br/>");
            var ignoreFlag=false;
            
            for(var j=0;j<ignoreList.length;j++) {
                if(sparkCode[i].indexOf(ignoreList[j])>=0) {
                    ignoreFlag=true;
                }
            }

            //if the line is valid, we now loop through all the translations and apply them to the line
            if(ignoreFlag==false) {
                
                line=sparkCode[i];
                for(var key in translation){
                    line=line.replace(key, translation[key]);
                }

                //now add the translated line to the JS code
                jsCode.push(line)
            }
        }

        for(var i=0;i<jsCode.length;i++) {
            var line=jsCode[i];
            for(var key in flagList) {
                if(line.indexOf(key)>=0) {
                    flagList[key]++;
                }
            }

            //we've initialized the cube object, let's find out its name
            if(flagList["Cube "]==1) {
                parts=line.split(' ');
                cubeObjectName=parts[1];
                // console.log("cube object name: "+cubeObjectName);
                flagList[cubeObjectName]="0";
                line="Cube "+cubeObjectName+";";
                jsCode[i]=line;
                flagList["Cube "]=0;
            }


            if(flagList["setup()"]==1) {
                if(line.indexOf("{")>=0) {
                    parts=line.split("{");
                    line=parts[0]+"{"+cubeObjectName+"=new Cube(this);"+"size(500,500, P3D);"+parts[1];
                    jsCode[i]=line;
                    flagList["setup()"]=0;
                }    
            }

            if(flagList["PVector"]==1) {
                line=line.replace("{", "new PVector(");
                line=line.replace("}", ")");
                jsCode[i]=line;     //replace the line in the code
                flagList["PVector"]=0;
            }

            if(flagList["draw()"]==1) {
                if(line.indexOf("{")>=0) {
                    parts=line.split("{");
                    line=parts[0]+"{"+canvasControl+parts[1];
                    jsCode[i]=line;
                    flagList["draw()"]=0;
                }
            }

            $("#jsCode").append(line+"<br/>");
            translatedCode+=line+"\n";
        }

        after();
    });
}