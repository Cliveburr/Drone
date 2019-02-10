V0 - Primeiro drone

- Frame
{
    F450 4-Axis
    https://www.aliexpress.com/item/F450-Multi-Copter-Quad-copter-Kit-Frame-QuadX-Quad-MultiCopter-KK-MK-MWC-Free-Shipping/32538293782.html?spm=a2g0s.9042311.0.0.yY2yIA
}

- Motor
{
    A2212 Brushless Motor
    https://www.aliexpress.com/item/4PCS-A2212-1000KV-1400KV-1800KV-Brushless-Motor-2-Pairs-1045-Propeller-Props-For-RC-F450-Multicopter/32808084650.html?spm=a2g0s.9042311.0.0.yY2yIA

    1400kv

    No load Current : 10 V : 0.5 A.
    Maximum current: 12A/60s.
    Apply to the ESC : 30A.
    Apply to the Lipo : 2 - 3S.
    Apply to the NiCD / NiMH : 6 - 9S.
    Apply to the Propeller : 8x4.5" 9x4.5" 10x4.5" 10x4.7".
    Apply to the three Propeller : 7x4.5" 8x4.5" 9x4.5".
    Does not contain Dimension : 28.0mm x 26.8mm .
    Does not contain Dimension : 28.0mm x 40.0mm .
    Shaft diameter : 3.175mm.
}

- Propeller
{
    10x4.5"
    (link - same as motor)

    ABS
}

- Batery
{
    Lipo 11.1V 3S 3000mAh 30C
    http://www.ebay.com/itm/11-1V-3000mAh-30C-Lipo-Battery-For-RC-Helicopter-Airplane-Hobby-Truck-Boat-New-/172450784074?epid=509880685&hash=item2826de234a:g:kEwAAOSw2xRYV1Ez


}

- Eletronic Speed Controller - ESC
{
    30A SimonK
    https://www.aliexpress.com/item/4pcs-lot-30A-SimonK-Prgramme-RC-Brushless-ESC-With-BEC-3A-For-Axis-Quadcopter-Multicopter-Wholesale/32225789649.html?spm=a2g0s.8937460.0.0.fGuakj

    Cont Current: 30A
    Burst Current: 35A
    BEC Model: Linear mode
    BEC Output: 5V2A
    Li-ion/Li0poly:2-4S,or Ni-MH/Ni-Cd:4-12NIMH
    Weight: 25g
    Size: 32*24*7mm
}

- Controller
{
    - Custom

    Module - Motors Controller
    {
        - Um microcontrolador PIC com 4 PWM
        - Comunicação serial
        - Nenhuma configuração
        - Recebe comandos para controlar a velocidade dos ESC por pulso PWM
    }

    Module - Principal
    {
        - Um microcontrolador ESP8266
        - Cria um Wifi onde o controle se conecta e recebe comandos via TCP
        - Roda programas, que coloca o controlador em um estado de reação,
            os programas só podem ser trocados se eles estiverem na condição ideal de cada programa
        Program - Fly
        {
            - Programa para efetuar o controle de voar o drone
            - Recebe comandos do tipo YAW, Pitch, Roll, Throttle
            - Recebe comandos de ajustes finos de calibragem dos motores
        }
        Program - Test and Calibrate
        {
            - Programa para efetuar o teste e calibração dos motores
        }
    }

    Interface
    {
        DroneV0Soft.App - Debug and Test All
        {

        }
        DroneV0Soft.Mobile - Fly controller for mobiles
        {
            
        }
    }
}