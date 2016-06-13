#!/usr/bin/env python
# -*- coding: utf-8 -*-

import smtplib
from email.MIMEText import MIMEText
from email.Header import Header
from email.Utils import formatdate

class Email_sender:
    u"""
    メール送信クラス．
    """

    def __init__(self, addr, password, host, port):
        self.addr = addr
        self.password = password
        self.host = host
        self.port = port

    def make_msg(self, subject, body, to_addr, charset="utf-8"):
        u"""
        メッセージを作成する関数，

        : パラメータ
        subject: unicode型．
        body: unicode型．
        to_addr: ascii．
        charset: エンコーディング．
        """
        print("MAKE MESSAGE")
        msg = MIMEText(body, "plain", charset)
        msg["Subject"] = Header(subject, charset)
        msg["From"] = self.addr
        msg["To"] = to_addr
        msg["Date"] = formatdate(localtime=True)
        return msg

    def send_msg(self, msg, to_addrs):
        u"""
        メッセージを送信する関数．

        : パラメータ
        msg: メッセージ．
        to_addrs: asciiのリスト．
        """
        print("SEND MESSAGE...")
        smtp = smtplib.SMTP_SSL(self.host, self.port)
        smtp.login(self.addr, self.password)
        smtp.sendmail(self.addr, to_addrs, msg.as_string())
        smtp.close()
        print("DONE")

    def send(self, subject, body, to_addr, charset="utf-8"):
        u"""
        メッセージを作成する関数，

        : パラメータ
        subject: unicode型．
        body: unicode型．
        to_addr: ascii．
        charset: エンコーディング．
        """
        msg = self.make_msg(subject, body, to_addr, charset)
        self.send_msg(msg, [to_addr])

class Gmail_sender(Email_sender):
    u"""
    Gメール送信クラス．
    """

    host = "smtp.gmail.com"
    port = 465

    def __init__(self, addr, password):
        Email_sender.__init__(self, addr, password, Gmail_sender.host, Gmail_sender.port)

def main():
    addr = "piyo@gmail.com"
    password = "pass"
    gmail = Gmail_sender(addr, password)
    subject = u"てすと"
    body = u"てすとです．"
    to_addr = "hoge@gmail.com"
    gmail.send(subject, body, to_addr)

if __name__ == "__main__":
    main()
