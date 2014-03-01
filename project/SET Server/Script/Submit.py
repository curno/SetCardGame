#!D:\python27\python.exe
print("Content-type: text/html;charset=utf-8\n\n")
import MySQLdb
import cgi
import base64

#connect to database and get cursor.
db_user = 'public'
db_host = 'localhost'
database = MySQLdb.connect(host = db_host, user = db_user, db = 'SETGAME')
cursor = database.cursor()

#get parameter
fields = cgi.FieldStorage()
name = fields['name'].value
score = fields['score'].value
elapsed_time = fields['elapsed_time'].value

#get the user_id for name
result_count = cursor.execute("select * from user where name = %s;", (name,))

#if no name exists in database, create new user.
if result_count == 0:
    cursor.execute("INSERT INTO USER VALUES (NULL, %s); ", (name,))
    database.commit();
    cursor.execute("SELECT id FROM USER WHERE name = %s;", (name,))

#insert (name, score, elapsed_time) into database
result = cursor.fetchone()
user_id = result[0];    
cursor.execute("INSERT INTO SCORE VALUES (NULL, %s, %s, %s);",
		(user_id, score, elapsed_time))
database.commit();
print('done')
