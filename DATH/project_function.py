import os
import cv2
import random
import math
import datetime
import string
import socket

string = "062201002944|233306838|Trần Đăng Huy|17112001|Nam|68/31 Hàm Nghi Tổ 8, Duy Tân, Thành phố Kon Tum, Kon Tum|22112021"
today = datetime.datetime.now()
year= today.year


def format(str):
  str = str[:2] + '/' + str[2:4] + '/' + str[4:]
  return str
def step_list(str):
  list = str.split('|')
  list[3] = format(list[3])
  list[6] = format(list[6])
  for i in range(len(list)):
    if list[i] == '':
      list[i] = None
  return list
def make_folder(directory_name):
    parent_dir = r"C:\Users\Huy\Documents\DATH\face_detect"
    path = os.path.join(parent_dir, directory_name)
    os.mkdir(path)
def count_dir(dir_path):
    count = 0
    # Iterate directory
    for path in os.listdir(dir_path):
        # check if current path is a folder
        if os.path.isdir(os.path.join(dir_path, path)):
            count += 1
    return count
def count_file(dir_path):
    count = 0
    for path in os.listdir(dir_path):
        # check if current path is a file
        if os.path.isfile(os.path.join(dir_path, path)):
            count += 1
    return count
def pin_random(length):
  ## storing strings in a list
  digits = [i for i in range(0, 10)]
  ## initializing a string
  random_str = ""
  ## we can generate any lenght of string we want
  for i in range(length):
    index = math.floor(random.random() * 10)
    random_str += str(digits[index])
  ## displaying the random string
  return random_str
def list_insert(checkin,checkout,length_of_stay, cccd, fname, birth, gender, email, phone, payment, room_id):
    list_pile = [None] * 11
    list_pile[0]= checkin
    list_pile[1] = checkout
    list_pile[2] = length_of_stay
    list_pile[3] = cccd
    list_pile[4] = fname
    list_pile[5] = birth
    list_pile[6] = gender
    list_pile[7] = email
    list_pile[8] = phone
    list_pile[9] = payment
    list_pile[10] = room_id
    return list_pile
def calendar_operator(day, month, year, leng_stay):
    year = int(year)
    calendar = {
        "Jan": 31,
        "Feb": 29 if year % 4 == 0 else 28,
        "Mar": 31,
        "Apr": 30,
        "May": 31,
        "Jun": 30,
        "Jul": 31,
        "Aug": 31,
        "Sep": 30,
        "Oct": 31,
        "Nov": 30,
        "Dec": 31
    }
    index = list(calendar).index(month)
    sum_day = int(leng_stay) + int(day)
    while(sum_day > list(calendar.values())[index]):
        print(index)
        sum_day -= list(calendar.values())[index]
        index += 1
        if (index == 12):
            index = 0
            year += 1
    month = list(calendar)[index]
    return str(sum_day)+ " " + month + " " + str(year)

def format_date(string_date):
    list_date = string_date.split()
    month = list_date[1]
    day = list_date[2]
    year = list_date[3]
    return day, month, year

def format_and_compare(date):
    currentDateAndTime = datetime.datetime.now()
    currentHour = currentDateAndTime.strftime("%H")
    calendar = {
        "Jan": 1,
        "Feb": 2,
        "Mar": 3,
        "Apr": 4,
        "May": 5,
        "Jun": 6,
        "Jul": 7,
        "Aug": 8,
        "Sep": 9,
        "Oct": 10,
        "Nov": 11,
        "Dec": 12
    }
    list_date = date.split()
    index = list(calendar).index(list_date[1])
    month = list(calendar.values())[index]
    check_in_date = datetime.date(int(list_date[2]), int(month), int(list_date[0]))
    if check_in_date == datetime.date.today():# and int(currentHour) >= 13 and int(currentHour) <= 15 :
        return True
    else:
        return False

