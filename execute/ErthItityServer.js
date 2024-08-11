const express = require('express');
const app = express();
const fs = require('fs');
const { error } = require('console');
const csvToJson = require('convert-csv-to-json');
const readline = require('readline');
const upgradeMtrDataTaskTimems = 1000;

let json = {};
let jsonstring = '';

app.listen(8181);
app.use(express.static(__dirname + "/public"));

app.get('/', (req, res) => {
    res.sendFile(__dirname + "/public/ErthItityIndex.html");
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
    json = csvToJson.fieldDelimiter(',').getJsonFromCsv(__dirname + '/data/output.csv');
    jsonstring = JSON.stringify(json);
    fs.writeFile(__dirname + '/public/json/ErthItityData.json', jsonstring, (err) => {
        if(err) throw err;
    });
    status = "0";
    fs.writeFile(__dirname + '/data/status.txt', status, (err) => {
        if(err) throw err;
    });
}, upgradeMtrDataTaskTimems);
