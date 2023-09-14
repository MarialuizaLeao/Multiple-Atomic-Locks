// No início da thread, ela deve receber como parâmetros seu tid, os tempos livre e crítico
//    e os recursos que ela necessita para completar seu trabalho
// Em segunda, ela deve executar as operações:
//  spend_time(tid,NULL,tlivre);
//  trava_recursos(recursos);     // a forma de representar os recursos é uma decisão do desenvolvedor
//  spend_time(tid,"C",tcritico);
//  libera_recursos();            // note que cada thread deve ser ter sua lista de recursos registrada em algum lugar