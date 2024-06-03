function consultaSede() {
    var vNome = document.getElementById('nome').value;
    var vEstado = document.getElementById('estado').value;
    $.ajax({
        url: "/sede/ObtemDadosConsultaAvancada",
        data: { nome: vNome, estado: vEstado },
        success: function (dados) {
            if (dados.erro != undefined) {
                alert(dados.msg);
            }
            else {
                document.getElementById('resultadoConsulta').innerHTML = '';
                document.getElementById('resultadoConsulta').innerHTML = dados;
            }
        },
    });

}

function consultaEstufa() {
    var vNome = document.getElementById('nome').value;
    var vSede = document.getElementById('sede').value;
    var vDtIni = document.getElementById('dataInicial').value;
    var vDtFin = document.getElementById('dataFinal').value;
    $.ajax({
        url: "/estufa/ObtemDadosConsultaAvancada",
        data: { nome: vNome, sede: vSede, dtIni: vDtIni, dtFin: vDtFin },
        success: function (dados) {
            if (dados.erro != undefined) {
                alert(dados.msg);
            }
            else {
                document.getElementById('resultadoConsulta').innerHTML = '';
                document.getElementById('resultadoConsulta').innerHTML = dados;
            }
        },
    });

}


function filtrarSensor() {
    var vSede = document.getElementById('sede').value;
    $.ajax({
        url: "/sensor/FiltraSensor",
        data: { sedeId: vSede },
        success: function (dados) {
            if (dados.erro != undefined) {
                alert(dados.msg);
            }
            else {
                document.getElementById('resultadoConsulta').innerHTML = '';
                document.getElementById('resultadoConsulta').innerHTML = dados;
            }
        },
    });

}

function cadastrarFiware() {
    var vId = document.getElementById('sensorId').value;
    $.ajax({
        url: "/sensor/CadastraFiware",
        data: { sensorId: vId },
        success: function (dados) {
            if (dados.erro != undefined) {
                Swal.fire({
                    title: dados.msg,
                    icon: "error",
                    showConfirmButton: false,
                    timer: 2500
                })
            }
            else {
                Swal.fire({
                    title: dados.msg,
                    icon: "sucess",
                    showConfirmButton: false,
                    timer: 2500
                })
            }
        },
    });

}


function loadChart(id) {
    $.ajax({
        url: "/sensor/DevolveValores",
        data: { sensorId: id },
        success: function (dados) {

            if (dados.valores != undefined) {
                const yValues = dados.valores;
                const xValues = dados.datas;

                new Chart("myChart", {
                    type: "line",
                    data: {
                        labels: xValues,
                        datasets: [{
                            label: "Sensor" + id,
                            backgroundColor: "rgba(0,0,255,0.7)",
                            borderColor: "rgba(0,0,255,0.1)",
                            data: yValues
                        }]
                    },
                    options: {
                        legend: { display: false },
                        title: {
                            display: true,
                            text: "Últimas 50 medições",
                            fontSize: 32
                        },
                        scales: {
                            yAxes: [{ ticks: { min: 0, max: 100 } }],
                        }
                    }
                });

                document.getElementById('max').innerHTML = dados.maxVal;
                document.getElementById('min').innerHTML = dados.minVal;
                document.getElementById('med').innerHTML = dados.med;
            }
            else {
                Swal.fire({
                    title: "Sem dados para o sensor",
                    text: "verificar o dispositivo",
                    icon: "info",
                    showConfirmButton: true
                }).then(() => {
                    location.href = '/Sensor/Index';
                });
            }

        },
    });
}

function loadChartEstufas(id) {
    $.ajax({
        url: "/estufa/DevolveValores",
        data: { estufaId: id },
        success: function (dados) {

            if (dados.valores != undefined) {
                const listValores = dados.valores;
                const xValues = dados.datas;
                const listIds = dados.listids;

                // Array para armazenar os datasets
                const datasets = [];

                const colors = ["rgba(0,0,255,0.7)", "rgba(255,0,0,0.7)", "rgba(0,255,0,0.7)", "rgba(100,0,100,0.7)", "rgba(0,100,100,0.7)"]

                // Itera sobre listValores e cria um dataset para cada valor
                listValores.forEach((valor, index) => {
                    datasets.push({
                        label: "Sensor" + listIds[index],
                        backgroundColor: colors[index],
                        borderColor: "rgba(0,0,255,0.1)",
                        data: valor
                    });
                });

                new Chart("myChart", {
                    type: "line",
                    data: {
                        labels: xValues,
                        datasets: datasets
                    },
                    options: {
                        legend: { display: true },
                        title: {
                            display: true,
                            text: "Últimas 50 medições da estufa",
                            fontSize: 32
                        },
                        scales: {
                            yAxes: [{ ticks: { min: 0, max: 100 } }],
                        }
                    }
                });

                document.getElementById('max').innerHTML = dados.maxVal;
                document.getElementById('min').innerHTML = dados.minVal;
                document.getElementById('med').innerHTML = dados.med;
            }
            else {
                Swal.fire({
                    title: "Sem dados para a estufa",
                    text: "verificar o dispositivo",
                    icon: "info",
                    showConfirmButton: true
                }).then(() => {
                    location.href = '/Estufa/Index';
                });
            }

        },
    });
}

function loadChartEstufasDatas(id) {
    var vDtIni = document.getElementById('dataInicial').value;
    var vDtFin = document.getElementById('dataFinal').value;
    $.ajax({
        url: "/estufa/DevolveValoresComDatas",
        data: { estufaId: id, dataDe: vDtIni, dataAte: vDtFin },
        success: function (dados) {

            if (dados.valores != undefined) {
                const listValores = dados.valores;
                const xValues = dados.datas;
                const listIds = dados.listids;

                // Array para armazenar os datasets
                const datasets = [];

                const colors = ["rgba(0,0,255,0.7)", "rgba(255,0,0,0.7)", "rgba(0,255,0,0.7)", "rgba(100,0,100,0.7)", "rgba(0,100,100,0.7)"]

                // Itera sobre listValores e cria um dataset para cada valor
                listValores.forEach((valor, index) => {
                    datasets.push({
                        label: "Sensor" + listIds[index],
                        backgroundColor: colors[index],
                        borderColor: "rgba(0,0,255,0.1)",
                        data: valor
                    });
                });

                new Chart("myChart", {
                    type: "line",
                    data: {
                        labels: xValues,
                        datasets: datasets
                    },
                    options: {
                        legend: { display: true },
                        title: {
                            display: true,
                            text: "Últimas 50 medições da estufa",
                            fontSize: 32
                        },
                        scales: {
                            yAxes: [{ ticks: { min: 0, max: 100 } }],
                        }
                    }
                });

                document.getElementById('max').innerHTML = dados.maxVal;
                document.getElementById('min').innerHTML = dados.minVal;
                document.getElementById('med').innerHTML = dados.med;
            }
            else {
                Swal.fire({
                    title: "Sem dados para a estufa",
                    text: "verificar o dispositivo",
                    icon: "info",
                    showConfirmButton: true
                }).then(() => {
                    location.href = '/Estufa/Index';
                });
            }

        },
    });
}
