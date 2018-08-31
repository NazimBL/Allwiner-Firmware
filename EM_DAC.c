
char kk;
char spi_read(unsigned spi_channel);
char readbuff[64],writebuff[64];
long measure1[32],measure2[32];

void adc_setup();
void timer_setup();
void InitTimer2();
unsigned long adc[10];
unsigned long l1[32],l2[32],l3[32],i1[32],i2[32],i3[32];
unsigned long vdc1[32],vdc2[32],idc[32];

unsigned long J[4],T[4];
unsigned long vs[32],is[32];
void solarRead();
void solarSend();
void readJT();
void sendVIsamples(char p);


unsigned int encoder[32];
char b[2];
void newCode();
char val1[4],val2[4];
unsigned long adc_result = 0;
unsigned char lowB,highB;

unsigned int last_encoder_state=0;

char count=0;
void sendSample(int f);
char p=0;
char send=0;

void Timer2_interrupt() iv IVT_INT_TIM2 {
  

  TIM2_SR.UIF = 0;
  if(count>31){

     count=0;
     send=1;

  }
  if(send==0){
  /*
  l1[count]=ADC1_Get_Sample(0);
  l2[count]=ADC1_Get_Sample(1);
  l3[count]=ADC1_Get_Sample(2);
  i1[count]=ADC1_Get_Sample(3);
  i2[count]=ADC1_Get_Sample(4);
  i3[count]=ADC1_Get_Sample(5);
  vdc1[count]=ADC1_Get_Sample(6);
  vdc2[count]=ADC1_Get_Sample(7);
  idc[count]=ADC1_Get_Sample(8);
  */
  vs[count]=ADC1_Get_Sample(0);
  is[count]=ADC1_Get_Sample(1);

  count++;
  
  }

  }
      

void main()
{

  HID_Enable(&readbuff,&writebuff);
  adc_setup();
  GPIO_Digital_Output(&GPIOC_BASE, _GPIO_PINMASK_13);
  timer_setup();
  UART1_Init(9600);
  Delay_ms(100);
  UART1_Write_Text("Setup \n");

  InitTimer2();



  while(1) 
  {
   
    GPIOC_ODR=0x0000;

    USB_Polling_Proc();
    kk = HID_Read();
    if(kk != 0)
    {
      solarRead();
     //emgDac();
     if(send==1)solarSend();
    }
    

  }
}

void emgDac(){

    if(send==1)newCode();
}

void solarSend(){
        if((char)readbuff[0]=='O'){

      for(p=0;p<2;p++)sendVIsamples(p);

         }else if((char)readbuff[0]=='O'){

      for(p=0;p<2;p++)sendVIsamples(p);

         }
}

void sendVIsamples(char p){

 char i=0,j=0;
      for(i=0;i<32;i++){
      if(p==0){
      lowB=vs[i]&0xFF;
      highB=vs[i]>>8;

      }else if(p==1){
      lowB=is[i]&0xFF;
      highB=is[i]>>8;
      }
      writebuff[j]=highB;
      j++;
      writebuff[j]=lowB;
      j++;
      }

      HID_Write(writebuff,64);
      send=0;

}

void solarRead(){

          GPIOC_ODR=0x0000;
          
          // if((char)readbuff[0]!='O')readJT();
          lowB=readbuff[0]&0xFF;
          highB=readbuff[1]>>8;

          UART1_Write_Text("ADC Value 1: ");
          sprintf(val1,"%d",lowB);
          UART1_Write_Text(val1);
          UART1_Write('\n');
          UART1_Write_Text("ADC Value 2: ");
          sprintf(val2,"%d",highB);
          UART1_Write_Text(val2);
          UART1_Write('\n');


}

void readJT(){
char i,j=0;
for(i=0;i<8;i++){

lowB=readbuff[i]&0xFF;
highB=readbuff[++i]>>8;
J[j]= lowB | highB<< 8;
j++;
}
j=0;
for(i=8;i<16;i++){

lowB=readbuff[i]&0xFF;
highB=readbuff[++i]>>8;
T[j]= lowB | highB<< 8;
j++;
}


}

void newCode(){

     if((char)readbuff[0]=='O'){

      for(p=0;p<9;p++)sendSample(p);
         
         }

}
void InitTimer2(){
  RCC_APB1ENR.TIM2EN = 1;
  TIM2_CR1.CEN = 0;
  //0 and 17999
  TIM2_PSC = 0;
  TIM2_ARR = 44999;
  //TIM2_ARR = 35999;
  NVIC_IntEnable(IVT_INT_TIM2);
  TIM2_DIER.UIE = 1;
  TIM2_CR1.CEN = 1;
}



void timer_setup(void)
{

     GPIO_Config(&GPIOB_BASE,_GPIO_PINMASK_6 | _GPIO_PINMASK_7,_GPIO_CFG_MODE_INPUT | _GPIO_CFG_PULL_UP);// setup port B.6 and B.7 as input with pull up
     RCC_APB1ENR.TIM4EN = 1;//  clock enable for timer_4
     TIM4_CR1bits.CEN=0;    //  timer_4 disable
     TIM4_SMCRbits.SMS=1;// encoder mode
     TIM4_CCERbits.CC1P=0;// capture is done on a rising edge for PB6
     TIM4_CCERbits.CC2P=0;// capture is done on a rising edge for PB7
     //1.6 Khz
     TIM4_PSC = 0;    //no prescaler
     TIM4_ARR=1000;  // encoder range from 0 to 1000
     TIM4_CCMR1_Inputbits.IC1F=15;// filter
     TIM4_CR1bits.CEN=1;  //  timer_4 enable
     
     RCC_APB1ENRbits.TIM3EN=1;
     
}

void adc_setup(){

  ADC_Set_Input_Channel(_ADC_CHANNEL_0);
  ADC_Set_Input_Channel(_ADC_CHANNEL_1);
  ADC_Set_Input_Channel(_ADC_CHANNEL_2);
  ADC_Set_Input_Channel(_ADC_CHANNEL_3);
  ADC_Set_Input_Channel(_ADC_CHANNEL_4);
  ADC_Set_Input_Channel(_ADC_CHANNEL_5);
  ADC_Set_Input_Channel(_ADC_CHANNEL_6);
  ADC_Set_Input_Channel(_ADC_CHANNEL_7);
  ADC_Set_Input_Channel(_ADC_CHANNEL_8);
  ADC1_Init();

}

void sendSample(int f){

      char i=0,j=0;
      for(i=0;i<32;i++){
      if(f==0){
      lowB=l1[i]&0xFF;
      highB=l1[i]>>8;

      }else if(f==1){
      lowB=l2[i]&0xFF;
      highB=l2[i]>>8;
      }else if(f==2){
      lowB=l3[i]&0xFF;
      highB=l3[i]>>8;
      }
      else if(f==3){
      lowB=i1[i]&0xFF;
      highB=i1[i]>>8;
      }else if(f==4){
      lowB=i2[i]&0xFF;
      highB=i2[i]>>8;
      }else if(f==5){
      lowB=i3[i]&0xFF;
      highB=i3[i]>>8;
      }else if(f==6){
      lowB=vdc1[i]&0xFF;
      highB=vdc1[i]>>8;
      }else if(f==7){
      lowB=vdc2[i]&0xFF;
      highB=vdc2[i]>>8;
      }else if(f==8){
      lowB=idc[i]&0xFF;
      highB=idc[i]>>8;
      }

      writebuff[j]=highB;
      j++;
      writebuff[j]=lowB;
      j++;
      }
      HID_Write(writebuff,64);
      send=0;

}


char spi_read(unsigned spi_channel){
char take, buffer;
     if(spi_channel==0){
        //b0 low
        GPIOB_ODR=0xFFFE;
        take = SPI1_Read(buffer);
     }else if(spi_channel==1){
           //b12 low
        GPIOB_ODR=0xEFFF;
        take = SPI1_Read(buffer);
     }else if(spi_channel==2){
         //a4 low
        GPIOA_ODR=0xFFF7;
        take = SPI1_Read(buffer);
     }else if(spi_channel==3){
     //a4 low
        GPIOA_ODR=0x7FFF;
        take = SPI1_Read(buffer);
     }
     //return all pins to high
     GPIOA_ODR=1;
     GPIOB_ODR=1;
     //unsigned buffer;
     //SPI1_Write(buffer);
     return take;
}

 }
