import smtplib, ssl
from project_function import pin_random
from email.message import EmailMessage

def send_code(receiver_email, password_random):
    port = 587
    smtp_server = "smtp.gmail.com"
    sender_email = "nguyenvutu010702@gmail.com"
    password = "tdyw tslt bicc ljch"
    message = """
    Your PIN to get into the room: {}
    This is your private code, don't share it arbitrarily.""".format(password_random)
    msg = EmailMessage()
    msg.set_content(message)
    msg['Subject'] = "PIN CODE"
    msg['From'] = "HOTEL SERVICE"
    msg['To'] = receiver_email
    context = ssl.create_default_context()
    with smtplib.SMTP(smtp_server, port) as server:
        server.ehlo()  # Can be omitted
        server.starttls(context=context)
        server.ehlo()  # Can be omitted
        server.login(sender_email, password)
        #server.sendmail(sender_email, receiver_email, message)
        server.send_message(msg)
send_code("longxichlo485@gmail.com","123456")
