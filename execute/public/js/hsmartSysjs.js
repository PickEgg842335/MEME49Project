const cDataInputUrl = "./json/ErthItityData.json";
const cEnvDisplayitem = {
    "Humidity" : ["室內溼度", "%"],
    "Temperature" : ["室內溫度", "℃"],
    "EnvLightSensePer" : ["室內亮度百分比", "%"],
};
const cItityDisplayitem = {
    "Acccle" : ["地表加速度", "g"],
    "Level" : ["地震警示燈狀態", ""],
    "GreenLightPer" : ["地震指示燈亮度", "%"],
};
const cDisOutputDisplayitem = {
    "Distence(mm)" : ["偵測車道口有無行人", "cm"],
    "DisLv" : ["車道口行人警示燈狀態", ""],
};
const cLightDisplayitem = {
    "EnvLightLv" : ["室內亮度級距", ""],
    "EnvLightStatus" : ["照明燈狀態", ""],
};
const cBzrDisplayitem = {
    "BzrStatus" : ["蜂鳴器狀態", ""],
};


window.onload = function()
{
    let ObjMtrDataInf = {
        ObjInput : {
            UpgradeFlag : false,
            AutoUpgrade : false,
            ObjIntervalUpgradeTaskId : {},
            UpgradeTimePeriod : 1000,
            timeoutCnt : 0,
            ObjInputData : {},
            ObjJsonButton : document.getElementById("idInputJson"),
            objInputTypeMsg : document.getElementById("idConnectType"),
        },
        ObjOutput : {
            ObjEnvOutputMsg : document.getElementById("idEnvOutputMsg"),
            ObjItityOutputMsg : document.getElementById("idItityOutputMsg"),
            ObjDisOutputMsg : document.getElementById("idDisOutputMsg"),
            ObjLightOutputMsg : document.getElementById("idLightOutputMsg"),
            ObjBzrOutputMsg : document.getElementById("idBzrOutputMsg"),
        },
        ObjField : {
            ItityWorkFlag : false,
            ItityIntervalUpgradeTaskId : {},
            ItityFlashCnt : 255,
            ItityCountSwitch : false,
            ItityRYFlag : false,
            DisWorkFlag : false,
            DisIntervalUpgradeTaskId : {},
            DisFlashCnt : 255,
            DisCountSwitch : false,
            DisRYFlag : false,
            BzrWorkFlag : false,
            BzrIntervalUpgradeTaskId : {},
            BzrFlashCnt : 255,
            BzrCountSwitch : false,
            ObjCtlField : document.getElementById("idField1"),
            ObjEnvField : document.getElementById("idField2"),
            ObjItityField : document.getElementById("idField3"),
            ObjDisField : document.getElementById("idField4"),
            ObjLightField: document.getElementById("idField5"),
            ObjBzrField : document.getElementById("idField6"),
        }
    };
    sfDisplayEnvData(ObjMtrDataInf);
    sfDisplayItityData(ObjMtrDataInf);
    sfDisplayDisOutputData(ObjMtrDataInf);
    sfDisplayLightOutputData(ObjMtrDataInf);
    sfDisplayBzrOutputData(ObjMtrDataInf);
    ObjMtrDataInf.ObjInput.ObjJsonButton.addEventListener("click", function(){
        sfStartPeriodMtrDataUpgrade(ObjMtrDataInf);
    });
}


function sfStartPeriodMtrDataUpgrade(ObjInf)
{
    if(ObjInf.ObjInput.AutoUpgrade == false)
    {
        ObjInf.ObjInput.ObjIntervalUpgradeTaskId = setInterval(() => {
            sfGetMtrDataFromServer(ObjInf);
        }, ObjInf.ObjInput.UpgradeTimePeriod);
        ObjInf.ObjInput.AutoUpgrade = true;
        ObjInf.ObjInput.ObjJsonButton.value = "Stop Monitor";
    }
    else
    {
        clearInterval(ObjInf.ObjInput.ObjIntervalUpgradeTaskId);
        ObjInf.ObjInput.AutoUpgrade = false;
        ObjInf.ObjInput.ObjJsonButton.value = "Start Monitor";
        ObjInf.ObjInput.objInputTypeMsg.innerHTML = `連線待命中`;
        ObjInf.ObjInput.objInputTypeMsg.style.color = "black";
        ObjInf.ObjField.ObjCtlField.style.backgroundColor = "rgb(255, 255, 255)";
    }
}


function sfGetMtrDataFromServer(ObjInf)
{
    fetch(cDataInputUrl)
        .then((response) => {
            ObjInf.ObjInput.objInputTypeMsg.innerHTML = `連線成功..數據更新中`;
            ObjInf.ObjInput.objInputTypeMsg.style.color = "green";
            ObjInf.ObjField.ObjCtlField.style.backgroundColor = "rgb(160, 255, 160)";
            ObjInf.ObjInput.timeoutCnt = 0;
            return response.json();
    })
        .then((result) => {
            ObjInf.ObjInput.ObjInputData = result[0];
            ObjInf.ObjInput.UpgradeFlag = true;
            sfDisplayEnvData(ObjInf);
            sfDisplayItityData(ObjInf);
            sfDisplayDisOutputData(ObjInf);
            sfDisplayLightOutputData(ObjInf);
            sfDisplayBzrOutputData(ObjInf);
    })    
        .catch((error) => {
            if(++ObjInf.ObjInput.timeoutCnt > 10)
            {
                clearInterval(ObjInf.ObjInput.ObjIntervalUpgradeTaskId);
                ObjInf.ObjInput.AutoUpgrade = false;
                ObjInf.ObjInput.ObjJsonButton.value = "Start Monitor";
                ObjInf.ObjInput.objInputTypeMsg.innerHTML = `已與Server斷線`;
                ObjInf.ObjInput.objInputTypeMsg.style.color = "red";
                ObjInf.ObjField.ObjCtlField.style.backgroundColor = "rgb(255, 160, 160)";
            }
            else if(ObjInf.ObjInput.timeoutCnt > 3)
            {
                ObjInf.ObjInput.objInputTypeMsg.innerHTML = `嘗試重新連線中..`;
                ObjInf.ObjInput.objInputTypeMsg.style.color = "rgb(255, 128, 0)";
                ObjInf.ObjField.ObjCtlField.style.backgroundColor = "rgb(255, 255, 160)";
            }
    });
}


function sfDisplayEnvData(ObjInf)
{
    let InpputValue;

    if(ObjInf.ObjInput.UpgradeFlag == false)
    {
        ObjInf.ObjOutput.ObjEnvOutputMsg.innerHTML = `監控關閉中`;
    }
    else
    {
        ObjInf.ObjOutput.ObjEnvOutputMsg.innerHTML = ``;
        for(let item in cEnvDisplayitem)
        {
            switch(item)
            {
                case "Humidity":
                    InpputValue = Math.round(Number(ObjInf.ObjInput.ObjInputData[item]) * 10) / 10;
                    break;
                case "Temperature":
                    InpputValue = Math.round(Number(ObjInf.ObjInput.ObjInputData[item]) * 10) / 10;
                    break;
                default:
                    InpputValue = ObjInf.ObjInput.ObjInputData[item];
                    break;
            }

            ObjInf.ObjOutput.ObjEnvOutputMsg.innerHTML += `${cEnvDisplayitem[item][0]} : <span style="color:blue">${InpputValue} ${cEnvDisplayitem[item][1]}</span><br>`;
        }
    }
}


function sfDisplayItityData(ObjInf)
{
    let InpputValue;
    let CheckTempValue;
    let GreenLightValue = '';

    if(ObjInf.ObjInput.UpgradeFlag == false)
    {
        ObjInf.ObjOutput.ObjItityOutputMsg.innerHTML = `監控關閉中`;
        ObjInf.ObjField.ObjItityField.style.backgroundColor = "rgb(255, 255, 255)";
    }
    else
    {
        ObjInf.ObjOutput.ObjItityOutputMsg.innerHTML = ``;
        for(let item in cItityDisplayitem)
        {
            switch(item)
            {
                case "Level":
                    InpputValue = Number(ObjInf.ObjInput.ObjInputData[item]);
                    if(InpputValue & 0x03)
                    {
                        if(InpputValue & 0x02)
                        {
                            ObjInf.ObjField.ItityRYFlag = true;
                            InpputValue = `<span style="color:red; background:white">●紅燈(強震)</span>`;
                        }
                        else if(InpputValue & 0x01)
                        {
                            ObjInf.ObjField.ItityRYFlag = false;
                            InpputValue = `<span style="color:rgb(255, 128, 0); background:white">●黃燈(中震)</span>`;
                        }

                        if(ObjInf.ObjField.ItityWorkFlag == false)
                        {
                            ObjInf.ObjField.ItityWorkFlag = true;
                            ObjInf.ObjField.ItityIntervalUpgradeTaskId = setInterval(() => {
                                if((ObjInf.ObjField.ItityFlashCnt == 64))
                                {
                                    ObjInf.ObjField.ItityCountSwitch = true;

                                }
                                else if(ObjInf.ObjField.ItityFlashCnt == 255)
                                {
                                    ObjInf.ObjField.ItityCountSwitch = false;
                                }

                                if(ObjInf.ObjField.ItityCountSwitch == false)
                                {
                                    ObjInf.ObjField.ItityFlashCnt--;
                                }
                                else if(ObjInf.ObjField.ItityCountSwitch == true)
                                {
                                    ObjInf.ObjField.ItityFlashCnt++;
                                }

                                if(ObjInf.ObjField.ItityRYFlag == true)
                                {
                                    ObjInf.ObjField.ObjItityField.style.backgroundColor = `rgb(255, ${ObjInf.ObjField.ItityFlashCnt}, ${ObjInf.ObjField.ItityFlashCnt})`;
                                }
                                else
                                {
                                    ObjInf.ObjField.ObjItityField.style.backgroundColor = `rgb(255, 255, ${ObjInf.ObjField.ItityFlashCnt})`;
                                }
                            }, 5);
                        }
                    }
                    else
                    {
                        InpputValue = `<span style="color:green; background:white">●綠燈(無地震)</span>`;
                        if(ObjInf.ObjField.ItityWorkFlag == true)
                        {
                            ObjInf.ObjField.ItityWorkFlag = false;
                            clearInterval(ObjInf.ObjField.ItityIntervalUpgradeTaskId);
                        }
                    }
                    break;
                case "GreenLightPer":
                    InpputValue = Number(ObjInf.ObjInput.ObjInputData[item]);
                    CheckTempValue = ObjInf.ObjInput.ObjInputData["Level"];
                    GreenLightValue = `rgb(${220 - InpputValue}, 255, ${220 - InpputValue})`;
                    if(CheckTempValue == 0)
                    {
                        ObjInf.ObjField.ObjItityField.style.backgroundColor = GreenLightValue;
                    }
                    break;
                default:
                    InpputValue = ObjInf.ObjInput.ObjInputData[item];
                    break;
            }

            ObjInf.ObjOutput.ObjItityOutputMsg.innerHTML += `${cItityDisplayitem[item][0]} : <span style="color:blue">${InpputValue} ${cItityDisplayitem[item][1]}</span><br>`;
        }
    }
}


function sfDisplayDisOutputData(ObjInf)
{
    let InpputValue;

    if(ObjInf.ObjInput.UpgradeFlag == false)
    {
        ObjInf.ObjOutput.ObjDisOutputMsg.innerHTML = `監控關閉中`;
    }
    else
    {
        ObjInf.ObjOutput.ObjDisOutputMsg.innerHTML = ``;
        for(let item in cDisOutputDisplayitem)
        {
            switch(item)
            {
                case "Distence(mm)":
                    InpputValue = Number(ObjInf.ObjInput.ObjInputData[item]) / 100;
                    break;
                case "DisLv":
                    InpputValue = Number(ObjInf.ObjInput.ObjInputData[item]);
                    if(InpputValue & 0x0C)
                    {
                        if(InpputValue & 0x08)
                        {
                            InpputValue = `<span style="color:red;background:white">●紅燈(行人在車道口)</span>`;
                            ObjInf.ObjField.DisRYFlag = true;
                        }
                        else if(InpputValue & 0x04)
                        {
                            InpputValue = `<span style="color:rgb(255, 128, 0);background:white">●黃燈(行人接近)</span>`;
                            ObjInf.ObjField.DisRYFlag = false;
                        }

                        if(ObjInf.ObjField.DisWorkFlag == false)
                        {
                            ObjInf.ObjField.DisWorkFlag = true;
                            ObjInf.ObjField.DisIntervalUpgradeTaskId = setInterval(() => {
                                if((ObjInf.ObjField.DisFlashCnt == 160))
                                {
                                    ObjInf.ObjField.DisCountSwitch = true;

                                }
                                else if(ObjInf.ObjField.DisFlashCnt == 255)
                                {
                                    ObjInf.ObjField.DisCountSwitch = false;
                                }

                                if(ObjInf.ObjField.DisCountSwitch == false)
                                {
                                    ObjInf.ObjField.DisFlashCnt--;
                                }
                                else if(ObjInf.ObjField.DisCountSwitch == true)
                                {
                                    ObjInf.ObjField.DisFlashCnt++;
                                }

                                if(ObjInf.ObjField.DisRYFlag == true)
                                {
                                    ObjInf.ObjField.ObjDisField.style.backgroundColor = `rgb(255, ${ObjInf.ObjField.DisFlashCnt}, ${ObjInf.ObjField.DisFlashCnt})`;
                                }
                                else
                                {
                                    ObjInf.ObjField.ObjDisField.style.backgroundColor = `rgb(255, 255, ${ObjInf.ObjField.DisFlashCnt})`;
                                }
                            }, 10);
                        }
                    }
                    else
                    {
                        InpputValue = `<span style="color:green;background:white">●綠燈(無行人)</span>`;
                        ObjInf.ObjField.ObjDisField.style.backgroundColor = "rgb(160, 255, 160)";
                        if(ObjInf.ObjField.DisWorkFlag == true)
                        {
                            ObjInf.ObjField.DisWorkFlag = false;
                            clearInterval(ObjInf.ObjField.DisIntervalUpgradeTaskId);
                        }
                    }
                    break;
                default:
                    InpputValue = ObjInf.ObjInput.ObjInputData[item];
                    break;
            }

            ObjInf.ObjOutput.ObjDisOutputMsg.innerHTML += `${cDisOutputDisplayitem[item][0]} : <span style="color:blue">${InpputValue} ${cDisOutputDisplayitem[item][1]}</span><br>`;
        }
    }
}


function sfDisplayLightOutputData(ObjInf)
{
    let InpputValue;
    let Temp;

    if(ObjInf.ObjInput.UpgradeFlag == false)
    {
        ObjInf.ObjOutput.ObjLightOutputMsg.innerHTML = `監控關閉中`;
    }
    else
    {
        ObjInf.ObjOutput.ObjLightOutputMsg.innerHTML = ``;
        for(let item in cLightDisplayitem)
        {
            switch(item)
            {
                case "EnvLightLv":
                    InpputValue = Number(ObjInf.ObjInput.ObjInputData[item]);
                    switch(InpputValue)
                    {
                        case 1:
                            InpputValue = `Lv1室外全黑`;
                            break;
                        case 2:
                            InpputValue = `Lv2室外光線昏暗`;
                            break;
                        case 3:
                            InpputValue = `Lv3室外光線普通`;
                            break;
                        case 4:
                            InpputValue = `Lv4室外光線充足`;
                            break;
                        case 5:
                            InpputValue = `Lv5室外光線明亮`;
                            break;
                    }
                    break;
                case "EnvLightStatus":
                    InpputValue = Number(ObjInf.ObjInput.ObjInputData[item]);
                    switch(InpputValue)
                    {
                        case 0:
                            InpputValue = `室內燈關閉`;
                            break;
                        case 1:
                            InpputValue = `室內燈開啟`;
                            break;
                    }
                    break;
                default:
                    InpputValue = ObjInf.ObjInput.ObjInputData[item];
                    break;
            }

            ObjInf.ObjOutput.ObjLightOutputMsg.innerHTML += `${cLightDisplayitem[item][0]} : <span style="color:blue">${InpputValue} ${cLightDisplayitem[item][1]}</span><br>`;
            if(Number(ObjInf.ObjInput.ObjInputData["EnvLightStatus"]) == 1)
            {
                ObjInf.ObjField.ObjLightField.style.backgroundColor = "rgb(255, 255, 160)";
            }
            else
            {
                Temp = Number(ObjInf.ObjInput.ObjInputData["EnvLightLv"]);
                ObjInf.ObjField.ObjLightField.style.backgroundColor = `rgb(${120 + Temp * 27}, ${120 + Temp * 27} , ${120 + Temp * 27})`;
            }
        }
    }
}


function sfDisplayBzrOutputData(ObjInf)
{
    let InpputValue;

    if(ObjInf.ObjInput.UpgradeFlag == false)
    {
        ObjInf.ObjOutput.ObjBzrOutputMsg.innerHTML = `監控關閉中`;
    }
    else
    {
        ObjInf.ObjOutput.ObjBzrOutputMsg.innerHTML = ``;
        for(let item in cBzrDisplayitem)
        {
            switch(item)
            {
                case "BzrStatus":
                    InpputValue = Number(ObjInf.ObjInput.ObjInputData[item]);
                    switch(InpputValue)
                    {
                        case 0:
                            InpputValue = `蜂鳴器關閉`;
                            if(ObjInf.ObjField.BzrWorkFlag == true)
                            {
                                ObjInf.ObjField.BzrWorkFlag = false;
                                clearInterval(ObjInf.ObjField.BzrIntervalUpgradeTaskId);
                                ObjInf.ObjField.ObjBzrField.style.backgroundColor = "white";
                            }
                            break;
                        case 1:
                            InpputValue = `蜂鳴器開啟`;
                            if(ObjInf.ObjField.BzrWorkFlag == false)
                            {
                                ObjInf.ObjField.BzrWorkFlag = true;
                                ObjInf.ObjField.BzrIntervalUpgradeTaskId = setInterval(() => {
                                    if((ObjInf.ObjField.BzrFlashCnt == 0))
                                    {
                                        ObjInf.ObjField.BzrCountSwitch = true;

                                    }
                                    else if(ObjInf.ObjField.BzrFlashCnt == 255)
                                    {
                                        ObjInf.ObjField.BzrCountSwitch = false;
                                    }

                                    if(ObjInf.ObjField.BzrCountSwitch == false)
                                    {
                                        ObjInf.ObjField.BzrFlashCnt--;
                                    }
                                    else if(ObjInf.ObjField.BzrCountSwitch == true)
                                    {
                                        ObjInf.ObjField.BzrFlashCnt++;
                                    }

                                    ObjInf.ObjField.ObjBzrField.style.backgroundColor = `rgb(255, ${ObjInf.ObjField.BzrFlashCnt}, 255)`;
                                }, 5);
                            }
                            break;
                    }
                    break;
                default:
                    InpputValue = ObjInf.ObjInput.ObjInputData[item];
                    break;
            }

            ObjInf.ObjOutput.ObjBzrOutputMsg.innerHTML += `${cBzrDisplayitem[item][0]} : <span style="color:blue">${InpputValue} ${cBzrDisplayitem[item][1]}</span><br>`;
        }
    }
}

