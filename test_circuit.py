import schemdraw
import schemdraw.elements as elm

with schemdraw.Drawing() as d:
    d.config(fontsize=12)
    IC555def = elm.Ic(pins=[elm.IcPin(name='TRG', side='left', pin='2'),
                            elm.IcPin(name='THR', side='left', pin='6'),
                            elm.IcPin(name='DIS', side='left', pin='7'),
                            elm.IcPin(name='CTL', side='right', pin='5'),
                            elm.IcPin(name='OUT', side='right', pin='3'),
                            elm.IcPin(name='RST', side='top', pin='4'),
                            elm.IcPin(name='Vcc', side='top', pin='8'),
                            elm.IcPin(name='GND', side='bot', pin='1'),],
                       edgepadW=.5,
                       edgepadH=1,
                       pinspacing=1.5,
                       leadlen=1,
                       label='555')
    d += (T := IC555def)
    
    d += (R1 := elm.Resistor().at(T.DIS).left().length(2).label("R1\n10kΩ"))
    d += (Vcc := elm.Vdd().at(R1.start).up())
    d += (R2 := elm.Resistor().at(R1.end).down().length(2).label("R2\n10kΩ"))
    d += (C := elm.Capacitor().at(R2.end).down().length(2).label("C\n48uF"))
    d += elm.Ground().at(C.end).down()

    
    
    d.draw()
