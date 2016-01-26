var sparkOutput = document.getElementById('sparkOutput');
var particleDevice = null;
var saberStatus = false;
var pastSaberStrength = 0;

spark.login({ username: 'USERNAME', password: 'PASSWORD' }, function (err, body) {
  sparkOutput.innerHTML = 'API call login completed on callback:' + body;
  spark.getDevice('ID_OF_PARTICLE', function (err, device) {
    sparkOutput.innerHTML += 'Device name: ' + device.name;
    particleDevice = device;
    SelectForceColor(null);
  });
});

var SelectForceColor = function (color) {
  if (particleDevice != null) {
    message.innerHTML = "Please wait for initialization";
    particleDevice.callFunction("selectColor", color, function (err, data) {
      if (err) {
        console.log('An error occurred:', err);
      } else {
        CallTurnOff();
        message.innerHTML = "Begin!";
      }
    });
  }
}

var output = document.getElementById('output');
var controller = new Leap.Controller();
var saberStrength = 0;

controller.connect();
this.controller.on('connect', function () {
  setInterval(
    function () {
      var frame = controller.frame();
      // output.innerHTML = 'Frame: ' + frame.id + '<br/>';
      for (var index in frame.hands) {
        var hand = frame.hands[index];
        // output.innerHTML += 'hand: ' + hand + '<br/>';
        // output.innerHTML += 'pinchStrength: ' + hand.pinchStrength;
        
        if(hand.grabStrength > 0.4)
        {
          saberStrength += 10;
          if(saberStrength > 100)
          {
            saberStrength = 100;
          }
        }else{
          saberStrength -= 10;
          if(saberStrength < 0)
          {
            saberStrength = 0;
          }
        }
        output.innerHTML = 'ForceStrength: ' + saberStrength;//hand.grabStrength;
        console.log(saberStrength);
        if (saberStrength > 0 && pastSaberStrength != saberStrength) {
          pastSaberStrength = saberStrength;
          // turn on the LEDs
          if (particleDevice != null) {
            particleDevice.callFunction("setStrength", saberStrength.toString(), function (err, data) {
              if (err) {
                console.log('An error occurred:', err);
              } else {
                console.log('setStrength:', data);
              }
            });
            saberStatus = true;
          }
        } else if (saberStrength < 0 && saberStatus) {
          // turn off the LEDs
          CallTurnOff();
        }
      }
      if (frame.hands.length == 0 && saberStatus) {
        CallTurnOff();
      }
    }, 500);
});

var CallTurnOff = function () {
  if (particleDevice != null) {
    particleDevice.callFunction("turnOff", null, function (err, data) {
      if (err) {
        console.log('An error occurred:', err);
      } else {
        console.log('turnOff:', data);
      }
    });
    saberStrength = 0;
  }
};