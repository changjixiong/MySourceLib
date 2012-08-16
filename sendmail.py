import smtplib
from email.mime.text import MIMEText


def SendMail(smtpServer, PORT, User, Pass, NeedLogin, From, To, Subject, Content):
    try:
        msg = MIMEText(Content)
        msg['Subject'] = Subject
        msg['From'] = From
        msg['To'] = To

        if NeedSSL=='1':
            s = smtplib.SMTP_SSL()
            s.connect(smtpServer,PORT)
        else:
            s = smtplib.SMTP()
            s.connect(smtpServer,PORT)

        if NeedLogin=='1':
            s.login(User, Pass)
    
        s.sendmail(From, To.split(';'), msg.as_string())
        s.quit()
        s.close()
        return True
    except Exception, x:
        print x
        return False



