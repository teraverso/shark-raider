function irPara(idDaTela) {
    document.querySelectorAll('.tela').forEach(t => t.classList.remove('ativa'));
    const proxima = document.getElementById(idDaTela);
    if (proxima) proxima.classList.add('ativa');
}

// Timer automático da Tela 1
window.onload = () => {
    setTimeout(() => {
        // Apenas muda se ainda estiver na tela 1
        if(document.getElementById('tela1').classList.contains('ativa')) {
            irPara('tela2');
        }
    }, 2000);
};
