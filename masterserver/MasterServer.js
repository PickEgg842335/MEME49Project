const express = require('express');
const app = express();
const fs = require('fs');
const { error } = require('console');
const csvToJson = require('convert-csv-to-json');
const readline = require('readline');

const cData1InputUrl = "http://192.168.1.113:8181/read";
const upgradeMtrDataTaskTimems = 500;

let json = [];
let jsonstring = '';

app.listen(80);
app.use(express.static(__dirname + "/public"));

app.get('/', (req, res) => {
    res.sendFile(__dirname + "/public/hsmartSysIndex.html");
});

app.get('/read', (req, res) => {
    res.send(jsonstring);
    res.end();
});

const upgradeMtrDataTask = setInterval(() => {
    (async () => {
        const fetch = await import('node-fetch');

        fetch.default(cData1InputUrl)
        .then((response) => {
            return response.json();
        })
        .then((result) => {
            json[0] = result[0];
            json[1] = {"Hello": 2,};
            jsonstring = JSON.stringify(json);
            fs.writeFile(__dirname + '/public/json/Monitor.json', jsonstring, (err) => {
                if(err) throw err;
            });
        })
        .catch((error) => {
        });
    })();
}, upgradeMtrDataTaskTimems);
