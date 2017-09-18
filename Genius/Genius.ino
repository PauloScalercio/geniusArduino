#define LED_VERDE 2
#define LED_AMARELO 3
#define LED_VERMELHO 4
#define LED_AZUL 5
#define INDEFINIDO -1

#define BOTAO_VERDE 8
#define BOTAO_AMARELO 9
#define BOTAO_VERMELHO 10
#define BOTAO_AZUL 11

#define BUZZER 13

#define UM_SEGUNDO 1000
#define MEIO_SEGUNDO 500

#define TAMANHO_SEQUENCIA 4

enum Estados{

  PRONTO_PARA_PROX_RODADA,
  USUARIO_RESPONDENDO,
  JOGO_FINALIZADO_SUCESSO,
  JOGO_FINALIZADO_FALHA
};

int sequenciaLuzes[TAMANHO_SEQUENCIA];

int rodada = 0;

int leds_respondidos = 0;

void setup() {
  // put your setup code here, to run once: 
  Serial.begin(9600);
  iniciaPortas();
  iniciaJogo();  

}

void iniciaJogo(){
  int jogo = analogRead(0);
  randomSeed(jogo);
  for(int i = 0; i < TAMANHO_SEQUENCIA; i++){
    sequenciaLuzes[i] = sorteiaCor();
  }
  
}

int sorteiaCor(){
  return random(LED_VERDE, LED_AZUL + 1);
}

void iniciaPortas(){
  pinMode(LED_VERDE,OUTPUT);
  pinMode(LED_AMARELO,OUTPUT);
  pinMode(LED_VERMELHO,OUTPUT);
  pinMode(LED_AZUL,OUTPUT);

  pinMode(BOTAO_VERDE, INPUT_PULLUP);
  pinMode(BOTAO_AMARELO, INPUT_PULLUP);
  pinMode(BOTAO_VERMELHO, INPUT_PULLUP);
  pinMode(BOTAO_AZUL, INPUT_PULLUP);

  pinMode(BUZZER, OUTPUT);
  
}
 
void loop() {

   switch(estadoAtual()){
    case PRONTO_PARA_PROX_RODADA:
      preparaNovaRodada();
      break;
    case USUARIO_RESPONDENDO:
      processaRespostaUsuario();
      break;
    case JOGO_FINALIZADO_SUCESSO:
      jogoFinalizadoSucesso();
      break;
    case JOGO_FINALIZADO_FALHA:
      jogoFinalizadoFalha();
      break;
   }
   
}

void preparaNovaRodada(){
   rodada++;
   leds_respondidos = 0;
   if(rodada <= TAMANHO_SEQUENCIA){
    tocaLedsRodada();
   }
}

void processaRespostaUsuario(){
  int resposta = checaRespostaJogador();

  if(resposta == INDEFINIDO){
    return;
  }

  if(resposta == sequenciaLuzes[leds_respondidos]){
    leds_respondidos++;
  }else{
    Serial.println("Resposta errada");
    rodada = TAMANHO_SEQUENCIA + 2;
  }
}

int estadoAtual(){
  if(rodada <= TAMANHO_SEQUENCIA){
    if(leds_respondidos == rodada){
      return PRONTO_PARA_PROX_RODADA;
    }else{
      return USUARIO_RESPONDENDO;
    }
  }else if (rodada == TAMANHO_SEQUENCIA + 1){
    return JOGO_FINALIZADO_SUCESSO;
  }else{
    return JOGO_FINALIZADO_FALHA;
  }
}

void tocaLedsRodada(){
  for(int i = 0; i < rodada; i++){
    piscaLed(sequenciaLuzes[i]); 
  }
}

int checaRespostaJogador(){

  if(digitalRead(BOTAO_VERDE) == LOW){
    return piscaLed(LED_VERDE);
  }
  if(digitalRead(BOTAO_AMARELO) == LOW){
    return piscaLed(LED_AMARELO);
  }
  if(digitalRead(BOTAO_VERMELHO) == LOW){
    return piscaLed(LED_VERMELHO);
  }
  if(digitalRead(BOTAO_AZUL) == LOW){
    return piscaLed(LED_AZUL);
  }

  return INDEFINIDO;
  
}


void jogoFinalizadoSucesso(){
  piscaLedSucesso(LED_VERDE);
  piscaLedSucesso(LED_AMARELO);
  piscaLedSucesso(LED_VERMELHO);
  piscaLedSucesso(LED_AZUL);
  
}

void jogoFinalizadoFalha(){

  tocaSom(300);
  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_AMARELO, HIGH);
  digitalWrite(LED_VERMELHO, HIGH);
  digitalWrite(LED_AZUL, HIGH);
  delay(MEIO_SEGUNDO);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AMARELO, LOW);
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_AZUL, LOW);
  delay(100);
}

int piscaLed(int portaLed){

  verificaSomDoLed(portaLed);
  
  digitalWrite(portaLed,HIGH);
  delay(UM_SEGUNDO);
  digitalWrite(portaLed,LOW);
  delay(MEIO_SEGUNDO);
  return portaLed;
}
int piscaLedSucesso(int portaLed){
 
  verificaSomDoLed(portaLed);
    
  digitalWrite(portaLed,HIGH);
  delay(MEIO_SEGUNDO);
  digitalWrite(portaLed,LOW);
  return portaLed;
}

void tocaSom(int frequencia){

  tone(BUZZER, frequencia, 100);
}

void verificaSomDoLed(int portaLed){

  switch (portaLed) {
    case LED_VERDE:
      tocaSom(2000);
      break;
    case LED_AMARELO:
      tocaSom(2200);
      break;
    case LED_VERMELHO:
      tocaSom(2400);
      break;
    case LED_AZUL:
      tocaSom(2400);
      break;
  }
}

