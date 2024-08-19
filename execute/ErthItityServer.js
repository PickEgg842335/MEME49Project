const express = require('express');
const app = express();
const fs = require('fs');
const { error } = require('console');
const csvToJson = require('convert-csv-to-json');
const readline = require('readline');
const osutils = require('os-utils');
const cors = require('cors');
const upgradeMtrDataTaskTimems = 500;

let json = [];
let jsonstring = '';

app.listen(8181);
app.use(express.static(__dirname + "/public"));

app.get('/', (req, res) => {
    res.sendFile(__dirname + "/public/hsmartSysIndex.html");
});

app.get('/read', (req, res) => {
    res.send(jsonstring);
    res.end();
});

const upgradeMtrDataTask = setInterval(() => {
    let status;
    status = "1";
    fs.writeFile(__dirname + '/data/status.txt', status, (err) => {
        if(err) throw err;
    });
    json[0] = csvToJson.fieldDelimiter(',').getJsonFromCsv(__dirname + '/data/output.csv')[0];
    jsonstring = JSON.stringify(json);
    // fs.writeFile(__dirname + '/public/json/ErthItityData.json', jsonstring, (err) => {
    //     if(err) throw err;
    // });
    status = "0";
    fs.writeFile(__dirname + '/data/status.txt', status, (err) => {
        if(err) throw err;
    });

    let ObjCpuInf = {
        "CPU Usage(%)" : "0",
        "CPU Free(%)" : "0",
        "Total Memory" : "0",
        "Free Memory" : "0",
        "Free Memory Percent" : "0",
        "CPU Temp" : "0",
    };

    osutils.cpuUsage(function(v){
        ObjCpuInf['CPU Usage(%)'] = String(Math.floor(v * 1000) / 10) + ' %';
        osutils.cpuFree(function(v){
            ObjCpuInf['CPU Temp'] = String(Math.floor(Number(fs.readFileSync('/sys/class/thermal/thermal_zone0/temp','utf-8') / 100)) / 10) + ' ℃';
            ObjCpuInf['CPU Free(%)'] = String(Math.floor(v * 1000) / 10) + ' %';
            ObjCpuInf['Total Memory'] = String(Math.floor(osutils.totalmem() / 100) / 10)+ ' GB';
            ObjCpuInf['Free Memory'] = String(Math.floor(osutils.freemem() / 100) / 10) + ' GB';
            ObjCpuInf['Free Memory Percent'] = String(Math.floor(osutils.freememPercentage() * 1000) / 10) + ' %';
            json[1] = ObjCpuInf;
            jsonstring = JSON.stringify(json);
            fs.writeFile(__dirname + '/public/json/ErthItityData.json', jsonstring, (err) => {
                if(err) throw err;
            });
        });
    });

}, upgradeMtrDataTaskTimems);
