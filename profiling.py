import time
from typing import Union

tstart = None
timer_name = None

def tic(msg: Union[str, None] = None):
    """Starts a timer, ended by ``toc()``

    :param msg: Message to come before the elapsed time statement, defaults to None
    :type msg: str, optional
    :raises Exception: If there is already an active ``tic``
    """
    global tstart, timer_name
    timer_name = msg
    if tstart is not None:
        raise Exception("tic() called with another tic() active!")
    tstart = time.perf_counter()


def toc(return_elapsed_seconds: bool = False) -> Union[None, float]:
    """Ends the timer began by ``tic()``, returns the elapsed time

    :param return_elapsed_seconds: Whether to return the time as well as printing it, defaults to False
    :type return_elapsed_seconds: bool, optional
    :raises Exception: If there's no active ``tic()`` timer running
    :return: The elapsed time in seconds, if requested
    :rtype: Union[None, float]
    """
    global tstart, timer_name
    if tstart is None:
        raise Exception("toc() called without an active tic()!")
    tend = time.perf_counter()
    telapsed = tend - tstart
    if not return_elapsed_seconds:
        print(
            f"{timer_name if timer_name is not None else 'Elapsed time'}: {telapsed:.2e} seconds"
        )
    tstart = None
    timer_name = None
    if return_elapsed_seconds:
        return telapsed

