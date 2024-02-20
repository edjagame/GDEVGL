import math

WINDOW_WIDTH = 640
WINDOW_HEIGHT = 360
ASPECT_RATIO = WINDOW_WIDTH/WINDOW_HEIGHT
PI = math.pi
index = 0

print("//DODECAGON")
print(f"{0:7.4f}, {0:7.4f},    0.0f, 1.0f, 1.0f, 1.0f, //0")
for i in range(12):
    print(f"{math.cos(math.pi/6*i)/ASPECT_RATIO/2:7.4f}, {math.sin(math.pi/6*i)/2:7.4f},    0.0f, 0.5f, 0.8f, 0.9f, //{i+1}")
print("")

print("//SNOWFLAKE INNER 1")
print(f"{0:7.4f}, {0:7.4f},    0.0f, 1.0f, 1.0f, 1.0f, //13")
for i in range(6):
    print(f"{math.cos(PI/3*i)/ASPECT_RATIO/8:7.4f}, {math.sin(PI/3*i)/8:7.4f},    0.0f, 1.0f, 1.0f, 1.0f, //{14+2*i}")
    print(f"{math.cos(PI/3*i+PI/12)/ASPECT_RATIO/10:7.4f}, {math.sin(PI/3*i+PI/12)/10:7.4f},    0.0f, 1.0f, 1.0f, 1.0f, //{15+2*i}")
print("")

print("//SNOWFLAKE INNER 2")
print(f"{0:7.4f}, {0:7.4f},    0.0f, 1.0f, 1.0f, 1.0f, //26")
for i in range(6):
    print(f"{math.cos(PI/3*i)/ASPECT_RATIO/8:7.4f}, {math.sin(PI/3*i)/8:7.4f},    0.0f, 1.0f, 1.0f, 1.0f, //{27+2*i}")
    print(f"{math.cos(PI/3*i-PI/12)/ASPECT_RATIO/10:7.4f}, {math.sin(PI/3*i-PI/12)/10:7.4f},    0.0f, 1.0f, 1.0f, 1.0f, //{28+2*i}")
print("")


print("//SNOWFLAKE OUTER 1")
print(f"{0:7.4f}, {0:7.4f},    0.0f, 1.0f, 1.0f, 1.0f, //39")
for i in range(6):
    print(f"{math.cos(PI/3*i+2*PI/24+PI/48)/ASPECT_RATIO/(11/3):7.4f}, {math.sin(PI/3*i+2*PI/24+PI/48)/(11/3):7.4f},    0.0f, 1.0f, 1.0f, 1.0f, //{40+2*i}")
    print(f"{math.cos(PI/3*i+4*PI/24-PI/48)/ASPECT_RATIO/3:7.4f}, {math.sin(PI/3*i+4*PI/24-PI/48)/3:7.4f},    0.0f, 1.0f, 1.0f, 1.0f, //{41+2*i}")
print("")

print("//SNOWFLAKE OUTER 2")
print(f"{0:7.4f}, {0:7.4f},    0.0f, 1.0f, 1.0f, 1.0f, //52")
for i in range(6):
    print(f"{math.cos(PI/3*i+4*PI/24+PI/48)/ASPECT_RATIO/3:7.4f}, {math.sin(PI/3*i+4*PI/24+PI/48)/3:7.4f},    0.0f, 1.0f, 1.0f, 1.0f, //{53+2*i}")
    print(f"{math.cos(PI/3*i+6*PI/24-PI/48)/ASPECT_RATIO/(10/3):7.4f}, {math.sin(PI/3*i+6*PI/24-PI/48)/(10/3):7.4f},    0.0f, 1.0f, 1.0f, 1.0f, //{54+2*i}")
print("")

print("//SNOWFLAKE OUTER 3")
for i in range(6):
    print(f"{math.cos(PI/3*i)/ASPECT_RATIO/4:7.4f}, {math.sin(PI/3*i)/4:7.4f},    0.0f, 1.0f, 1.0f, 1.0f, //{65+3*i}")
    print(f"{math.cos(PI/3*i+PI/6)/ASPECT_RATIO/(10/3):7.4f}, {math.sin(PI/3*i+PI/6)/(10/3):7.4f},    0.0f, 1.0f, 1.0f, 1.0f, //{66+3*i}")
    print(f"{math.cos(PI/3*i+PI/6)/ASPECT_RATIO/(5.0/2.0):7.4f}, {math.sin(PI/3*i+PI/6)/(5.0/2.0):7.4f},    0.0f, 1.0f, 1.0f, 1.0f, //{67+3*i}")
print("")

print("//SNOWFLAKE OUTER 4")
for i in range(6):
    print(f"{math.cos(PI/3*i+PI/3)/ASPECT_RATIO/4:7.4f}, {math.sin(PI/3*i+PI/3)/4:7.4f},    0.0f, 1.0f, 1.0f, 1.0f, //{83+3*i}")
    print(f"{math.cos(PI/3*i+PI/6)/ASPECT_RATIO/(10/3):7.4f}, {math.sin(PI/3*i+PI/6)/(10/3):7.4f},    0.0f, 1.0f, 1.0f, 1.0f, //{84+3*i}")
    print(f"{math.cos(PI/3*i+PI/6)/ASPECT_RATIO/(5.0/2.0):7.4f}, {math.sin(PI/3*i+PI/6)/(5.0/2.0):7.4f},    0.0f, 1.0f, 1.0f, 1.0f, //{85+3*i}")
print("")

print("//DODECAGON 2.1")
for i in range(12):
    if i==3:
        print(f"{math.cos(PI/6*i)/ASPECT_RATIO/(16/10):7.4f}, {math.sin(PI/6*i)/(16/10):7.4f},    0.0f, 0.5f, 0.5f, 0.5f, //{i+101}")
    else:
        print(f"{math.cos(PI/6*i)/ASPECT_RATIO/(19/10):7.4f}, {math.sin(PI/6*i)/(19/10):7.4f},    0.0f, 0.5f, 0.5f, 0.5f, //{i+101}")
print("")

print("//DODECAGON 2.2")
for i in range(12):
    print(f"{math.cos(PI/6*i)/ASPECT_RATIO/(15/10):7.4f}, {math.sin(PI/6*i)/(15/10):7.4f},    0.0f, 0.5f, 0.5f, 0.5f, //{i+113}")
print("")

print("//TOP")
print(f"{math.cos(PI/2-PI/24)/ASPECT_RATIO/(7/4):7.4f}, {0.75:7.4f},    0.0f, 0.5f, 0.5f, 0.5f, //{125}")
print(f"{math.cos(PI/2+PI/24)/ASPECT_RATIO/(7/4):7.4f}, {0.75:7.4f},    0.0f, 0.5f, 0.5f, 0.5f, //{126}")
print(f"{0:7.4f}, {0.9:7.4f},    0.0f, 0.5f, 0.5f, 0.5f, //{127}")
print(f"{0:7.4f}, {1.0:7.4f},    0.0f, 0.5f, 0.5f, 0.5f, //{128}")
print("")

print("//BOTTOM")
print(f"{0:7.4f}, {-1.0:7.4f},    0.0f, 0.5f, 0.5f, 0.5f, //{129}")
print("")

print("//SPIKES")
print(f"{math.cos(PI/3*2+2*PI/24)/ASPECT_RATIO/(17/10):7.4f}, {math.sin(PI/3*2+2*PI/24)/(17/10):7.4f},    0.0f, 0.5f, 0.5f, 0.5f, //{130}")
print(f"{math.cos(PI/3*2+6*PI/24)/ASPECT_RATIO/(17/10):7.4f}, {math.sin(PI/3*2+6*PI/24)/(17/10):7.4f},    0.0f, 0.5f, 0.5f, 0.5f, //{131}")
print(f"{math.cos(PI/3*2+PI/6)/ASPECT_RATIO/(1):7.4f}, {math.sin(PI/3*2+PI/6)/(1):7.4f},    0.0f, 0.5f, 0.5f, 0.5f, //{132}")
print(f"{math.cos(PI/3*0+2*PI/24)/ASPECT_RATIO/(17/10):7.4f}, {math.sin(PI/3*0+2*PI/24)/(17/10):7.4f},    0.0f, 0.5f, 0.5f, 0.5f, //{133}")
print(f"{math.cos(PI/3*0+6*PI/24)/ASPECT_RATIO/(17/10):7.4f}, {math.sin(PI/3*0+6*PI/24)/(17/10):7.4f},    0.0f, 0.5f, 0.5f, 0.5f, //{134}")
print(f"{math.cos(PI/3*0+PI/6)/ASPECT_RATIO/(1):7.4f}, {math.sin(PI/3*0+PI/6)/(1):7.4f},    0.0f, 0.5f, 0.5f, 0.5f, //{135}")
print("")

print(f"{math.cos(7*PI/6)/ASPECT_RATIO/(19/10):7.4f}, {math.sin(7*PI/6)/(19/10):7.4f},    0.0f, 0.5f, 0.5f, 0.5f, //{136}")
print(f"{math.cos(6*PI/6)/ASPECT_RATIO/(19/10):7.4f}, {math.sin(6*PI/6)/(19/10):7.4f},    0.0f, 0.5f, 0.5f, 0.5f, //{137}")
print(f"{math.cos(7*PI/6)/ASPECT_RATIO/(13/10):7.4f}, {math.sin(7*PI/6)/(13/10):7.4f},    0.0f, 0.5f, 0.5f, 0.5f, //{138}")
print(f"{math.cos(PI)/ASPECT_RATIO/(1):7.4f}, {math.sin(PI)/(1):7.4f},    0.0f, 0.5f, 0.5f, 0.5f, //{139}")
print(f"{math.cos(11*PI/6)/ASPECT_RATIO/(19/10):7.4f}, {math.sin(11*PI/6)/(19/10):7.4f},    0.0f, 0.5f, 0.5f, 0.5f, //{140}")
print(f"{math.cos(0*PI/6)/ASPECT_RATIO/(19/10):7.4f}, {math.sin(0*PI/6)/(19/10):7.4f},    0.0f, 0.5f, 0.5f, 0.5f, //{141}")
print(f"{math.cos(11*PI/6)/ASPECT_RATIO/(13/10):7.4f}, {math.sin(11*PI/6)/(13/10):7.4f},    0.0f, 0.5f, 0.5f, 0.5f, //{142}")
print(f"{math.cos(0)/ASPECT_RATIO/(1):7.4f}, {math.sin(0)/(1):7.4f},    0.0f, 0.5f, 0.5f, 0.5f, //{143}")
print("")

print("//SPIKES DESIGN")
print(f"{math.cos(PI/3*2+3*PI/24)/ASPECT_RATIO/(15/10):7.4f}, {math.sin(PI/3*2+3*PI/24)/(15/10):7.4f},    0.0f, 0.8f, 0.8f, 0.8f, //{144}")
print(f"{math.cos(PI/3*2+5*PI/24)/ASPECT_RATIO/(15/10):7.4f}, {math.sin(PI/3*2+5*PI/24)/(15/10):7.4f},    0.0f, 0.8f, 0.8f, 0.8f, //{145}")
print(f"{math.cos(PI/3*2+PI/6)/ASPECT_RATIO/(11/10):7.4f}, {math.sin(PI/3*2+PI/6)/(11/10):7.4f},    0.0f, 0.8f, 0.8f, 0.8f, //{146}")
print(f"{math.cos(PI/3*0+3*PI/24)/ASPECT_RATIO/(15/10):7.4f}, {math.sin(PI/3*0+3*PI/24)/(15/10):7.4f},    0.0f, 0.8f, 0.8f, 0.8f, //{147}")
print(f"{math.cos(PI/3*0+5*PI/24)/ASPECT_RATIO/(15/10):7.4f}, {math.sin(PI/3*0+5*PI/24)/(15/10):7.4f},    0.0f, 0.8f, 0.8f, 0.8f, //{148}")
print(f"{math.cos(PI/3*0+PI/6)/ASPECT_RATIO/(11/10):7.4f}, {math.sin(PI/3*0+PI/6)/(11/10):7.4f},    0.0f, 0.8f, 0.8f, 0.8f, //{149}")
print("")

print(f"{math.cos(6.9*PI/6)/ASPECT_RATIO/(18/10):7.4f}, {math.sin(6.9*PI/6)/(18/10):7.4f},    0.0f, 0.8f, 0.8f, 0.8f, //{150}")
print(f"{math.cos(6.1*PI/6)/ASPECT_RATIO/(18/10):7.4f}, {math.sin(6.1*PI/6)/(18/10):7.4f},    0.0f, 0.8f, 0.8f, 0.8f, //{151}")
print(f"{math.cos(6.9*PI/6)/ASPECT_RATIO/(27/20):7.4f}, {math.sin(6.9*PI/6)/(27/20):7.4f},    0.0f, 0.8f, 0.8f, 0.8f, //{152}")
print(f"{math.cos(6.1*PI/6)/ASPECT_RATIO/(21/20):7.4f}, {math.sin(6.1*PI/6)/(21/20):7.4f},    0.0f, 0.8f, 0.8f, 0.8f, //{153}")
print(f"{math.cos(11.1*PI/6)/ASPECT_RATIO/(18/10):7.4f}, {math.sin(11.1*PI/6)/(18/10):7.4f},    0.0f, 0.8f, 0.8f, 0.8f, //{154}")
print(f"{math.cos(-0.1*PI/6)/ASPECT_RATIO/(18/10):7.4f}, {math.sin(-0.1*PI/6)/(18/10):7.4f},    0.0f, 0.8f, 0.8f, 0.8f, //{155}")
print(f"{math.cos(11.1*PI/6)/ASPECT_RATIO/(27/20):7.4f}, {math.sin(11.1*PI/6)/(27/20):7.4f},    0.0f, 0.8f, 0.8f, 0.8f, //{156}")
print(f"{math.cos(-0.1*PI/6)/ASPECT_RATIO/(21/20):7.4f}, {math.sin(-0.1*PI/6)/(21/20):7.4f},    0.0f, 0.8f, 0.8f, 0.8f, //{157}")

print("")
print(f"{math.cos(PI/3*4)/ASPECT_RATIO/(17/10):7.4f}, {math.sin(PI/3*4)/(17/10):7.4f},    0.0f, 0.8f, 0.8f, 0.8f, //{158}")
print(f"{math.cos(PI/3*5)/ASPECT_RATIO/(17/10):7.4f}, {math.sin(PI/3*5)/(17/10):7.4f},    0.0f, 0.8f, 0.8f, 0.8f, //{159}")
print(f"{math.cos(PI*3/2)/ASPECT_RATIO/(17/10):7.4f}, {math.sin(PI*3/2)/(17/10):7.4f},    0.0f, 0.8f, 0.8f, 0.8f, //{160}")
print(f"{0:7.4f}, {-0.9:7.4f},    0.0f, 0.8f, 0.8f, 0.8f, //{161}")
print("\n\n################################################################################################################\n\n")

print("//DODECAGON")
for i in range(12-1):
    print(f"{0},{i+2},{i+1},")
print("0,1,12,")
print("")

print("//SNOWFLAKE INNER 1")
for i in range(6):
    print(f"{13},{2*i+15},{2*i+14},")
print("")

print("//SNOWFLAKE INNER 2")
for i in range(6):
    print(f"{26},{2*i+28},{2*i+27},")
print("")

print("//SNOWFLAKE OUTER 1")
for i in range(6):
    print(f"{39},{2*i+41},{2*i+40},")
print("")

print("//SNOWFLAKE OUTER 2")
for i in range(6):
    print(f"{52},{2*i+54},{2*i+53},")
print("")

print("//SNOWFLAKE OUTER 3")
for i in range(6):
    print(f"{3*i+65},{3*i+67},{3*i+66},")
print("")

print("//SNOWFLAKE OUTER 4")
for i in range(6):
    print(f"{3*i+83},{3*i+85},{3*i+84},")
print("")

print("//DODECAGON 2.1")
for i in range(12-1):
    print(f"{i+101},{i+102},{i+113},")
print(f"112,101,124,")
print("")

print("//DODECAGON 2.2")
for i in range(12-1):
    print(f"{i+102},{i+114},{i+113},")
print(f"101,113,124,")
print("")

print("//TOP")
print(f"115,116,125,")
print(f"116,117,126,")
print(f"125,117,127,")
print(f"127,115,126,")
print(f"128,117,127,")
print(f"127,115,128,")
print("")

print("//BOTTOM")
print("129,121,122,")
print("129,122,123,")
print("")

print("//SPIKES")
print("130,131,132,")
print("133,134,135,")
print("136,137,138,")
print("137,138,139,")
print("140,141,142,")
print("141,142,143,")
print("")

print("//SPIKES DESIGN")
print("144,145,146,")
print("147,148,149,")
print("150,151,152,")
print("151,152,153,")
print("154,155,156,")
print("155,156,157,")
print("")
print("161,160,157,")
print("160,161,158,")
print("")