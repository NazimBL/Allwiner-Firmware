 void usbTask(){
        try {
            if(checkUsb()){

                short m[]=MyUSB.usbCom(stmUsb,MainActivity.this);
                
                double res[]=new double[10];
                int r[]=new int[10];
                for(int i=0;i<9;i++){
                    res[i]=DB.convert(m[i],i);
                    r[i]=(int)res[i];

                }

                speedText.setText(""+m[9]);
                gauge3.setValue(m[9]);
                for(int i=0;i<3;i++){
                    if(r[i]<10)vac_text[i].setText("00"+r[i]);
                    else if(r[i]>=10 && r[i]<100)vac_text[i].setText("0"+r[i]);
                    else vac_text[i].setText(""+r[i]);
                }
                int j=0;
                for(int i=3;i<6;i++){
                    if(res[i]<10)iac_text[j].setText("0"+res[i]);
                    iac_text[j].setText(""+res[i]);
                    j++;
                }
                dc_text[0].setText(""+(r[7]-r[6]));
                if(res[8]<10)dc_text[1].setText("0"+res[8]);
                else dc_text[1].setText(""+res[8]);




            }else Toast.makeText(MainActivity.this,"no USB connected ",Toast.LENGTH_LONG).show();

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }
