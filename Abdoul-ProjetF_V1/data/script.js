
function updateSensorData() {
    fetch('/get-sensor-data')
        .then(response => response.text())
            .then(data => {
                    const [temperature, humidity, co2] = data.split(',');
                    document.getElementById('temperature').innerText = temperature;
                    document.getElementById('humidity').innerText = humidity;
                    document.getElementById('co2').innerText = co2;
                });
           }

        // Charger les données lorsque la page est chargée
    window.onload = updateSensorData;

        // Charger les données lorsque la page est actualisée
    window.onbeforeunload = function() {
        updateSensorData();
    };