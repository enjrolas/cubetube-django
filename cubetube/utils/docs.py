from bs4 import BeautifulSoup
with open ("../templates/cubetube/docs-raw.html", "r") as rawFile:
    data=rawFile.read()
print(data)
soup = BeautifulSoup(data)
