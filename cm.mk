$(call inherit-product, vendor/cm/config/common_full_phone.mk)

$(call inherit-product, vendor/cm/config/nfc_enhanced.mk)

$(call inherit-product, device/samsung/kltekor/full_kltekor.mk)

PRODUCT_DEVICE := kltekor
PRODUCT_NAME := cm_kltekor
