import ConfigParser

IPAddress=""
config=ConfigParser.ConfigParser()
config.read("spExecutor.ini")
IPAddress=config.get("db","IP")


config.set("db","lastNum",lastNum)
config.write(open("OLDGATest.ini","w"))
