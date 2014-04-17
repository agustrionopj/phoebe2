import pyfits as pf
from phoebe.atmospheres.passbands import *
import glob
import argparse
from phoebe.utils import utils



logger = logging.getLogger("ATM.CREATEPBFITS")


class Passband(object):
    def __init__(self,**kwargs):
        # make dictionary with data columns
        self.columns = {'WAVELENGTH':[],'RESPONSE':[]}
        
        # make dictionary with default headers
        # 'WAVLEFF' is computed and added at the end

        self.headers = {
            'SYSTEM':'',
            'PASSBAND':'',
            'SOURCE': '',
            'VERSION': '1.0',
            'EXTNAME': '',
            'DETTYPE':'flux',
            'COMMENTS': '',
            'ZPFLUX':'' ,
            'ZPFLUXUN':'erg/s/cm2/AA',
            'ZPMAG': '0',
            'ZPMAGTYP': 'Undefined'
            }

        # fill the dictionaries with kwargs
        for key in kwargs.keys():
            if key in self.header.keys():
                self.headers[key]=kwargs[key]
            elif arg in self.columns.keys():
                self.columns[key]=kwargs[key]

    def set_header(self,key,val,force=False):
        """Set a header item. It has to be one of the predefined headers,
        else if you really want to add a non-standard keyword, put force=True"""
        if force or key.upper() in self.headers.keys():
            self.headers[key.upper()]=val
        
    def set_column(self,key,val,force=False):
        """Set a data column. It has to be one of wavelength and response,
        else if you really want to add a non-standard keyword, put force=True"""
        if force or key.upper() in self.columns.keys():
            self.columns[key.upper()]=val

    def get_header(self,key):
        return self.headers[key.upper()]

    def get_headers(self):
        return self.headers

    def get_column(self,key):
        return self.columns[key.upper()]

    def get_columns(self):
        return self.columns
    
    def load_ascii(self, file, forceheaders=False):
        """Load an ascii file. All standard headers and the wavelength+response arrays 
        are processed. Any further columns and header items are discarded unless you
        use forceheaders=True, which forces all headers to be stored."""
        
        # get headers
        f = open(file,'r')
        headers = [line.strip('\n #').split(None,1) 
                for line in f.readlines() if line[0]=='#']
        
        for line in headers:
            if len(line)==2:
                key, val = line[0],line[1]
                self.set_header(key,val,force=forceheaders)            

        # get columns
        wavl, resp = np.loadtxt(file,usecols=(0,1),converters={
            0: lambda s: float(s.strip(',')), 1: lambda s: float(s.strip(','))},unpack=True)
        self.set_column('WAVELENGTH',wavl)
        self.set_column('RESPONSE',resp)
        
    def load_ascii_phoebe1(self,file):
        """Load an ascii file in the style of Phoebe 1. On top of the normal load_ascii(), this
        scales the wavelenght array using WLFACTOR, it converts the keyword values to the phoebe2
        definitions, and computes the effective wavelength."""
        # load the file in a temporary passband instance that has all header items
        logger.debug('Using Phoebe1 conversions for {0}'.format(file))

        temp_pb = Passband()
        temp_pb.load_ascii(file, forceheaders=True)
        
        # standard load
        self.load_ascii(file)
        
        # scale wavelengths
        wavescale = float(temp_pb.get_header('WLFACTOR'))
        self.set_column('WAVELENGTH',self.get_column('WAVELENGTH')*wavescale)
        
        # convert keywords
        # -- PASS_SET --> SYSTEM
        self.set_header('SYSTEM',temp_pb.get_header('PASS_SET'))
        # -- REFERENCE --> Source
        self.set_header('SOURCE',temp_pb.get_header('REFERENCE'))
        
        
        # compute effwavelength
        self.set_header('WAVLEFF',self.compute_eff_wave(),force=True)

    def compute_eff_wave(self):
        """Sets effective wavelength to WAVLEFF header. Can only be computed after filling the
         arrays."""
        det_type = self.get_header('DETTYPE').lower()
        
        # eff_wave_arrays takes ccd or bol as det_type, we convert our setting first
        det_type_translated = {'flux':'ccd','energy':'bol'}[det_type]
        resparray = (self.get_column('WAVELENGTH'),self.get_column('RESPONSE'))
        return eff_wave_arrays(resparray=resparray, model=None, det_type=det_type_translated)
        

def convert_ascii_to_fits(asciifiles,outfile,format=None):
    """Convert a list of asciifiles to one .fits file. Format can be None or phoebe1, in which
    case conversions of header keywords etc are performed to make the phoebe1 files compatible with
    phoebe2."""
    passbandcollection = []
    for file in asciifiles:
        p = Passband()
        logger.info('Adding {0}'.format(file))

        if format==None:
            p.load_ascii(file)
        elif format.lower()=='phoebe1':
            p.load_ascii_phoebe1(file)
        passbandcollection.append(p)

    write_passbands_to_fits(passbandlist=passbandcollection,outfile=outfile)    
    logger.info('Saved {0}'.format(outfile))

def write_passbands_to_fits(passbandlist,outfile):
    """Write a list of Passband instances to a fits file."""
    # create HDUlist and add empty primary
    hdulist = pf.HDUList([])
    hdulist.append(pf.PrimaryHDU())

    # create HDU for every passband
    for p in passbandlist:
        cols = []
        for key in p.get_columns():
            cols.append(pf.Column(name=key, format='D', array=p.get_column(key)))
        
        hdu = pf.new_table(pf.ColDefs(cols))

        head = hdu.header
        for key in sorted(p.get_headers()):
            head.set(key, str(p.get_header(key)))
        
        # set name of the extension
        head.set('EXTNAME', "{}.{}_v{}".format(p.get_header('system'),
                                p.get_header('passband'),p.get_header('version')))

        hdulist.append(hdu)

    # -- Write file
    hdulist.writeto(outfile,clobber=True)
            
    
    
    
if __name__ == "__main__":
    logger = utils.get_basic_logger()
    files = sorted(glob.glob('ptf/phoebe1set/*.ptf'))
    outfile = 'ptf/phoebe2_standardset.fits'
    convert_ascii_to_fits(files,outfile,format='phoebe1')