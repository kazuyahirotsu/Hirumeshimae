def on_button_pressed_a():
    global mode
    mode += 1
    if mode == 5:
        mode = 0
    switch_algorithm()
input.on_button_pressed(Button.A, on_button_pressed_a)

def switch_algorithm():
    if mode == 0:
        huskylens.init_mode(protocolAlgorithm.ALGORITHM_FACE_RECOGNITION)
        basic.show_string("F")
    if mode == 1:
        huskylens.init_mode(protocolAlgorithm.ALGORITHM_OBJECT_TRACKING)
        basic.show_string("O")
    if mode == 2:
        huskylens.init_mode(protocolAlgorithm.ALGORITHM_LINE_TRACKING)
        basic.show_string("L")
    if mode == 3:
        huskylens.init_mode(protocolAlgorithm.ALGORITHM_COLOR_RECOGNITION)
        basic.show_string("C")
    if mode == 4:
        huskylens.init_mode(protocolAlgorithm.ALGORITHM_TAG_RECOGNITION)
        basic.show_string("T")
mode = 0
basic.show_icon(IconNames.CHESSBOARD)
huskylens.init_i2c()
mode = 3
switch_algorithm()
huskylens.init_mode(protocolAlgorithm.ALGORITHM_COLOR_RECOGNITION)

def on_forever():
    huskylens.request()
    if huskylens.isAppear_s(HUSKYLENSResultType_t.HUSKYLENS_RESULT_BLOCK):
        if huskylens.is_appear(1, HUSKYLENSResultType_t.HUSKYLENS_RESULT_BLOCK):
            basic.show_number(1)
            serial.write_value("xc", huskylens.reade_box(1, Content1.X_CENTER))
            serial.write_value("yc", huskylens.reade_box(1, Content1.Y_CENTER))
            serial.write_value("w", huskylens.reade_box(1, Content1.WIDTH))
            serial.write_value("h", huskylens.reade_box(1, Content1.HEIGHT))
    elif huskylens.isAppear_s(HUSKYLENSResultType_t.HUSKYLENS_RESULT_ARROW):
        if huskylens.is_appear(1, HUSKYLENSResultType_t.HUSKYLENS_RESULT_ARROW):
            basic.show_number(1)
            serial.write_value("xb", huskylens.reade_arrow(1, Content2.X_ORIGIN))
            serial.write_value("yb", huskylens.reade_arrow(1, Content2.Y_ORIGIN))
            serial.write_value("xe", huskylens.reade_arrow(1, Content2.X_TARGET))
            serial.write_value("ye", huskylens.reade_arrow(1, Content2.Y_TARGET))
    else:
        basic.show_icon(IconNames.NO)
    basic.pause(500)
basic.forever(on_forever)