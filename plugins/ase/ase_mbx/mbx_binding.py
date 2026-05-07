import ctypes
import os
import ctypes.util
from ctypes import c_double, c_int, c_char_p, POINTER

import numpy as np

KCAL_PER_MOL_TO_EV = 0.0433641153087705


class MBXLibrary:
    def __init__(self, mbx_home: str | None = None):
        self.mbx_home = mbx_home or os.getenv("MBX_HOME")
        self.lib = self._load_library()
        self._configure_signatures()
        self.initialized = False

    def _load_library(self):
        dlopen_mode = getattr(os, "RTLD_LAZY", None)

        if self.mbx_home:
            lib_dir = os.path.join(self.mbx_home, "lib")
            candidates = [
                os.path.join(lib_dir, "libmbx.so"),
                os.path.join(lib_dir, "libmbx.dylib"),
                os.path.join(lib_dir, "libmbx.dll"),
            ]
            for path in candidates:
                if os.path.exists(path):
                    if dlopen_mode is None:
                        return ctypes.CDLL(path)
                    return ctypes.CDLL(path, mode=dlopen_mode)

        # Fallback to the system library search path when MBX_HOME/lib is not usable.
        lib_name = ctypes.util.find_library("mbx")
        if lib_name:
            if dlopen_mode is None:
                return ctypes.CDLL(lib_name)
            return ctypes.CDLL(lib_name, mode=dlopen_mode)

        raise FileNotFoundError(
            "Could not find libmbx. Set MBX_HOME before running, "
            "or ensure libmbx is on the system library path."
        )

    def _configure_signatures(self):
        self.lib.initialize_system_py_.argtypes = [
            POINTER(c_double),  # coords
            POINTER(c_int),     # nat_monomers
            POINTER(c_char_p),  # atom_names
            POINTER(c_char_p),  # monomer_names
            POINTER(c_int),     # nmon
            c_char_p,           # json_file
        ]
        self.lib.initialize_system_py_.restype = None

        self.lib.get_energy_g_.argtypes = [
            POINTER(c_double),  # coords
            POINTER(c_int),     # nat
            POINTER(c_double),  # energy
            POINTER(c_double),  # gradients
        ]
        self.lib.get_energy_g_.restype = None

        self.lib.get_energy_pbc_g_.argtypes = [
            POINTER(c_double),  # coords
            POINTER(c_int),     # nat
            POINTER(c_double),  # box
            POINTER(c_double),  # energy
            POINTER(c_double),  # gradients
        ]
        self.lib.get_energy_pbc_g_.restype = None

        self.lib.get_energy_decomp_.argtypes = [
            POINTER(c_double),  # coords
            POINTER(c_int),     # nat
            POINTER(c_double),  # E1b
            POINTER(c_double),  # E2b
            POINTER(c_double),  # E3b
            POINTER(c_double),  # E4b
            POINTER(c_double),  # Edisp
            POINTER(c_double),  # Ebuck
            POINTER(c_double),  # Eelec
        ]
        self.lib.get_energy_decomp_.restype = None

        self.lib.get_energy_decomp_pbc_.argtypes = [
            POINTER(c_double),  # coords
            POINTER(c_int),     # nat
            POINTER(c_double),  # box
            POINTER(c_double),  # E1b
            POINTER(c_double),  # E2b
            POINTER(c_double),  # E3b
            POINTER(c_double),  # E4b
            POINTER(c_double),  # Edisp
            POINTER(c_double),  # Ebuck
            POINTER(c_double),  # Eelec
        ]
        self.lib.get_energy_decomp_pbc_.restype = None

        self.lib.get_electrostatic_energy_decomp_.argtypes = [
            POINTER(c_double),  # coords
            POINTER(c_int),     # nat
            POINTER(c_double),  # Eperm
            POINTER(c_double),  # Eind
        ]
        self.lib.get_electrostatic_energy_decomp_.restype = None

        self.lib.get_electrostatic_energy_decomp_pbc_.argtypes = [
            POINTER(c_double),  # coords
            POINTER(c_int),     # nat
            POINTER(c_double),  # box
            POINTER(c_double),  # Eperm
            POINTER(c_double),  # Eind
        ]
        self.lib.get_electrostatic_energy_decomp_pbc_.restype = None

        self.lib.get_xyz_.argtypes = [POINTER(c_double)]
        self.lib.get_xyz_.restype = None

        self.lib.get_charges_.argtypes = [POINTER(c_double)]
        self.lib.get_charges_.restype = None

        self.lib.get_induced_dipoles_.argtypes = [POINTER(c_double)]
        self.lib.get_induced_dipoles_.restype = None

        self.lib.get_polarizabilities_.argtypes = [POINTER(c_double)]
        self.lib.get_polarizabilities_.restype = None

        self.lib.get_potential_and_electric_field_on_points_.argtypes = [
            POINTER(c_double),  # coords
            POINTER(c_double),  # phi
            POINTER(c_double),  # ef
            POINTER(c_int),     # npoints
        ]
        self.lib.get_potential_and_electric_field_on_points_.restype = None

        self.lib.get_external_field_contribution_to_energy_.argtypes = [POINTER(c_double)]
        self.lib.get_external_field_contribution_to_energy_.restype = None

        self.lib.get_virial_.argtypes = [POINTER(c_double)]
        self.lib.get_virial_.restype = None

        self.lib.finalize_system_.argtypes = []
        self.lib.finalize_system_.restype = None

    def initialize_system(self, coords, nat_monomers, atom_names, monomer_names, json_file):
        coords_flat = np.ascontiguousarray(coords, dtype=np.float64).ravel()
        crd = coords_flat.ctypes.data_as(POINTER(c_double))

        nat_arr = np.ascontiguousarray(nat_monomers, dtype=np.int32)
        nats = nat_arr.ctypes.data_as(POINTER(c_int))

        at_bytes = [bytes(a, encoding="utf8") for a in atom_names]
        ats = (c_char_p * len(at_bytes))(*at_bytes)

        mon_bytes = [bytes(m, encoding="utf8") for m in monomer_names]
        mons = (c_char_p * len(mon_bytes))(*mon_bytes)

        nmon = c_int(len(mon_bytes))
        jsn = c_char_p(bytes(json_file, encoding="utf8"))

        self.lib.initialize_system_py_(crd, nats, ats, mons, ctypes.byref(nmon), jsn)
        self.initialized = True

    def get_energy_forces(self, coords):
        coords_flat = np.ascontiguousarray(coords, dtype=np.float64).ravel()
        crd = coords_flat.ctypes.data_as(POINTER(c_double))
        nat = c_int(len(coords_flat) // 3)
        energy = c_double(0.0)
        grads = np.zeros_like(coords_flat)
        grad_ptr = grads.ctypes.data_as(POINTER(c_double))

        self.lib.get_energy_g_(crd, ctypes.byref(nat), ctypes.byref(energy), grad_ptr)
        return energy.value, grads.reshape((-1, 3))

    def get_energy_component_breakdown(self, coords):
        coords_flat = np.ascontiguousarray(coords, dtype=np.float64).ravel()
        crd = coords_flat.ctypes.data_as(POINTER(c_double))
        nat = c_int(len(coords_flat) // 3)
        components = [c_double(0.0) for _ in range(7)]

        self.lib.get_energy_decomp_(
            crd,
            ctypes.byref(nat),
            *(ctypes.byref(component) for component in components),
        )
        return tuple(component.value for component in components)

    def get_energy_forces_pbc(self, coords, cell_vectors):
        coords_flat = np.ascontiguousarray(coords, dtype=np.float64).ravel()
        box_flat = np.ascontiguousarray(cell_vectors, dtype=np.float64).ravel()
        crd = coords_flat.ctypes.data_as(POINTER(c_double))
        box = box_flat.ctypes.data_as(POINTER(c_double))
        nat = c_int(len(coords_flat) // 3)
        energy = c_double(0.0)
        grads = np.zeros_like(coords_flat)
        grad_ptr = grads.ctypes.data_as(POINTER(c_double))

        self.lib.get_energy_pbc_g_(crd, ctypes.byref(nat), box, ctypes.byref(energy), grad_ptr)
        return energy.value, grads.reshape((-1, 3))

    def get_energy_component_breakdown_pbc(self, coords, cell_vectors):
        coords_flat = np.ascontiguousarray(coords, dtype=np.float64).ravel()
        box_flat = np.ascontiguousarray(cell_vectors, dtype=np.float64).ravel()
        crd = coords_flat.ctypes.data_as(POINTER(c_double))
        box = box_flat.ctypes.data_as(POINTER(c_double))
        nat = c_int(len(coords_flat) // 3)
        components = [c_double(0.0) for _ in range(7)]

        self.lib.get_energy_decomp_pbc_(
            crd,
            ctypes.byref(nat),
            box,
            *(ctypes.byref(component) for component in components),
        )
        return tuple(component.value for component in components)

    def get_electrostatic_energy_breakdown(self, coords):
        coords_flat = np.ascontiguousarray(coords, dtype=np.float64).ravel()
        crd = coords_flat.ctypes.data_as(POINTER(c_double))
        nat = c_int(len(coords_flat) // 3)
        permanent = c_double(0.0)
        induced = c_double(0.0)

        self.lib.get_electrostatic_energy_decomp_(
            crd,
            ctypes.byref(nat),
            ctypes.byref(permanent),
            ctypes.byref(induced),
        )
        return permanent.value, induced.value

    def get_electrostatic_energy_breakdown_pbc(self, coords, cell_vectors):
        coords_flat = np.ascontiguousarray(coords, dtype=np.float64).ravel()
        box_flat = np.ascontiguousarray(cell_vectors, dtype=np.float64).ravel()
        crd = coords_flat.ctypes.data_as(POINTER(c_double))
        box = box_flat.ctypes.data_as(POINTER(c_double))
        nat = c_int(len(coords_flat) // 3)
        permanent = c_double(0.0)
        induced = c_double(0.0)

        self.lib.get_electrostatic_energy_decomp_pbc_(
            crd,
            ctypes.byref(nat),
            box,
            ctypes.byref(permanent),
            ctypes.byref(induced),
        )
        return permanent.value, induced.value

    def get_electrostatic_site_coordinates(self, site_count):
        xyz = np.zeros(3 * int(site_count), dtype=np.float64)
        self.lib.get_xyz_(xyz.ctypes.data_as(POINTER(c_double)))
        return xyz.reshape((-1, 3))

    def get_electrostatic_site_charges(self, site_count):
        charges = np.zeros(int(site_count), dtype=np.float64)
        self.lib.get_charges_(charges.ctypes.data_as(POINTER(c_double)))
        return charges

    def get_induced_site_dipoles(self, site_count):
        dipoles = np.zeros(3 * int(site_count), dtype=np.float64)
        self.lib.get_induced_dipoles_(dipoles.ctypes.data_as(POINTER(c_double)))
        return dipoles.reshape((-1, 3))

    def get_electrostatic_site_polarizabilities(self, site_count):
        polarizabilities = np.zeros(int(site_count), dtype=np.float64)
        self.lib.get_polarizabilities_(polarizabilities.ctypes.data_as(POINTER(c_double)))
        return polarizabilities

    def get_potential_and_electric_field_at_points(self, points):
        points_array = np.ascontiguousarray(points, dtype=np.float64).reshape((-1, 3))
        coords = points_array.ravel()
        npoints = c_int(len(points_array))
        phi = np.zeros(len(points_array), dtype=np.float64)
        electric_field = np.zeros(3 * len(points_array), dtype=np.float64)

        self.lib.get_potential_and_electric_field_on_points_(
            coords.ctypes.data_as(POINTER(c_double)),
            phi.ctypes.data_as(POINTER(c_double)),
            electric_field.ctypes.data_as(POINTER(c_double)),
            ctypes.byref(npoints),
        )
        return phi, electric_field.reshape((-1, 3))

    def get_external_field_energy_contribution(self):
        energy = c_double(0.0)
        self.lib.get_external_field_contribution_to_energy_(ctypes.byref(energy))
        return energy.value

    def get_virial(self):
        virial = np.zeros(9, dtype=np.float64)
        self.lib.get_virial_(virial.ctypes.data_as(POINTER(c_double)))
        return virial.reshape((3, 3))

    def finalize(self):
        if self.initialized:
            self.lib.finalize_system_()
            self.initialized = False
