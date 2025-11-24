import csv
table = ''
i = 0
with open('/root/FQC_LOG/FQC_Result.csv') as csvFile:
    reader = csv.reader(csvFile, delimiter=',')
    for row in reader:
        i +=1
        
        if i == 1:
            table += '<tr><td bgcolor = "#90EE90"><b>{}</b></td><td bgcolor = "#90EE90"><b>{}</b></td><td bgcolor = "#CD5C5C"><b>{}</b></td><td bgcolor = "#CD5C5C"><b>{}</b></td></tr>'.format(row[0],row[1],row[2],row[3])

        elif i == 2:
            table += '<tr><td bgcolor = "#FFFF00"><b>SNo</b></td><td bgcolor = "#FFFF00"><b>Testcase Name</b></td><td bgcolor = "#FFFF00"><b>Testcase Output</b></td><td bgcolor = "#FFFF00"><b>Result</b></td></tr>'

        else:
            if (row[3] == 'FAIL'):
                table += '<tr><td>{}</td> <td>{}</td> <td>{}</td> <td bgcolor = "#CD5C5C">{}</td> </tr>'.format(row[0],row[1],row[2],row[3])
            else:
                table += '<tr><td>{}</td> <td>{}</td> <td>{}</td> <td bgcolor = "#90EE90">{}</td> </tr>'.format(row[0], row[1], row[2], row[3])


html = """
<table border=3>
  {}
</table>
""".format(table)

with open('/root/FQC_LOG/FQC_html.html', 'w') as f:
    f.write(html)
    f.close()
