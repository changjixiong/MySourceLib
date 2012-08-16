import adodbapi

cur=None
con=None
conStr=''

conStr="Data Source="+IPAddress+";\
                Trusted_Connection=No;\
                Initial Catalog="+DBName+";\
                provider=sqloledb;\
                User ID="+User+";Password="+Password
con=adodbapi.connect(conStr)
cur=con.cursor()        
cur.execute(SQLCMD)
con.commit()     

if cur is not None:
	cur.close()
if con is not None:
	con.close()

columns='['+'],['.join(DicForSQL.keys())+']'
cur.execute("insert into OLDGAData ("+columns+") values("+'?,'*(len(DicForSQL) -1)+"?)", DicForSQL.values())


cur.execute("select top 1 id,EmailAddress,EmailTitle,EmailContent \
                        from AlarmMsg_Email \
                        where id>"+str(dealedID)\
                        +"order by id")
row = cur.fetchone()
if row != None:
	dealedID=int(row[0])
	EmailAddress=row[1]
	EmailTitle=row[2]
	EmailContent=row[3]
