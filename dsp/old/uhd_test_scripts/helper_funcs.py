import uhd

#for clock and time use :
#    0: internal oscillator
#    1: external source
#    2: internal GPSDO
def _get_arg_string(val):
    if val == 1:
        return 'external'
    elif val == 2:
        return 'gpsdo'
    else:
        return ''


def setup_usrp_device(usrp:uhd.usrp.MultiUSRP, settings:dict = None):
    default_settings = {
        "center_freq": 100e6,
        "sample_rate": 1e6,
        "gain" : 10,
        "clock_source": 0,
        "time_source": 0 }
    if settings == None: settings = default_settings
    
    center_freq = settings["center_freq"]
    sample_rate = settings["sample_rate"]
    gain = settings["gain"]

    usrp.set_rx_rate(sample_rate, 0)
    usrp.set_rx_freq(uhd.libpyuhd.types.tune_request(center_freq), 0)
    usrp.set_rx_gain(gain, 0)
    
    clock_arg = _get_arg_string(settings['clock_source'])
    if not(clock_arg == ''):
        usrp.set_clock_source(clock_arg)

    time_arg = _get_arg_string(settings['time_source']) 
    if not(time_arg == ''):
        usrp.set_time_source(time_arg)


def calc_num_samples(time:int, usrp:uhd.usrp.MultiUSRP):
    sample_rate = usrp.get_rx_rate()
    return int(sample_rate * time)


