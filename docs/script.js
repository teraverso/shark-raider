document.addEventListener('DOMContentLoaded', () => {
    const terminal = document.getElementById('terminal-content');
    
    const logs = [
        "> Starting ESP32 hardware...",
        "> Initializing WiFi Hunter module...",
        "> Scanning frequencies...",
        "> Target Found: [REDACTED_SSID]",
        "> RSSI: -45dBm",
        "> Capturing packets...",
        "> Educational mode: ENABLED",
        "> Logs saved to internal flash memory.",
        "> [ALERT] Research environment detected.",
        "> Shark Raider is ready for commands..."
    ];

    let logIndex = 0;

    function updateTerminal() {
        // Adiciona nova linha
        const newLine = document.createElement('p');
        newLine.className = 'line';
        newLine.style.marginBottom = '5px';
        newLine.textContent = logs[logIndex];
        terminal.appendChild(newLine);

        logIndex++;

        // Reseta o terminal se acabar os logs (loop infinito)
        if (logIndex >= logs.length) {
            setTimeout(() => {
                terminal.innerHTML = '<p class="line">> Rebooting Shark OS...</p>';
                logIndex = 0;
            }, 3000);
        }

        // Auto-scroll para o final do terminal
        terminal.scrollTop = terminal.scrollHeight;
    }

    // Roda a atualização a cada 1.5 segundos
    setInterval(updateTerminal, 1500);
});

// Efeito de suavidade no scroll do menu
document.querySelectorAll('a[href^="#"]').forEach(anchor => {
    anchor.addEventListener('click', function (e) {
        e.preventDefault();
        document.querySelector(this.getAttribute('href')).scrollIntoView({
            behavior: 'smooth'
        });
    });
});