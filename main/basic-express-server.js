/* 
For this server to work you need to have node installed on your system. You also have to install some dependecies. 
 Execute this command in terminal:
    `npm install express body-parser`
and then run the project by command 
    `node basic-express-server.js`
*/

const express = require('express');
const bodyParser = require('body-parser');

let ledState = false;

const app = express();

const port = 4001;

app.use(bodyParser.urlencoded({extended: true}));
app.use(bodyParser.json());

app.post('/sensors/reed-switch', (req, res) => {
    const data = req.body;

    console.log(`REED SWITCH DATA: `, data);
    res.status(200).send('Data received successfully!');
});

app.post("/sensors/dht", (req, res) => {
    const data = req.body;

    console.log('DHT SENSOR DATA: ', data);
    res.status(200).send('Data received successfully!');
});

app.post("/sensors/smoke-sensor", (req, res) => {
    const data = req.body;

    console.log('SMOKE SENSOR DATA: ', data);
    res.status(200).send('Data received successfully!');
});

app.post("/sensors/pir-sensor", (req, res) => {
    const data = req.body;

    console.log('PIR SENSOR DATA: ', data);
    res.status(200).send('Data received successfully!');
});

app.get('/light-state', (req, res) => {
    res.json({lightState: ledState ? 'on' : 'off'});
})

app.post('/light-state', (req, res) => {
    const {state} = req.body;
    if (state !== undefined) {
        let redLetState = state === 'true' ? true : state === 'false' ? false : null;

        if (ledState !== null) {
            ledState = redLetState;

            res.status(200).json({message: 'LED state updated successfully.'});
        } else {
            res.status(400).json({error: 'Invalid LED state provided.'});
        }
    } else {
        res.status(400).json({error: 'No LED state provided.'});
    }
})

app.listen(port, () => {
    console.log(`Server is running on port ${port}`);
});
