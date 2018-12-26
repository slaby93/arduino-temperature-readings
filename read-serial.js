const fs = require('fs');
const SerialPort = require('serialport');
const serialPort = new SerialPort('/dev/ttyACM0', {
  baudRate: 20000,
  parser: new SerialPort.parsers.Readline("\n")
});

serialPort.on('data', (() => {
  let lastSecond = null
  let index = 0;
  return data => {
    const parsedData = data.toString('utf8')  
      .split(",");
    const [time] = parsedData
    const timeInSeconds = (time / 1000).toFixed(1)
    if( timeInSeconds === lastSecond) { return }
    lastSecond = timeInSeconds
    index++
    fs.appendFile('data.xxx', `[${index}, ${parsedData[1]}, ${parsedData[2]}, ${parsedData[3]}, ${parsedData[4]}],\n`, () => {})
  }
})());