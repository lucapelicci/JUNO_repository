import os
import re
import argparse

def crea_liste_per_run(cartella_finale,TriggerType):
    percorso_base = "/junofs/production/commissioning/rtraw/2025"
    cartella_completa = os.path.join(percorso_base, cartella_finale)

    cartella_list = os.path.join(os.getcwd(), "list")

    if not os.path.exists(cartella_completa):
        print(f"Errore: La cartella {cartella_completa} non esiste.")
        return
    if not os.path.exists(cartella_list):
        os.makedirs(cartella_list)

    run_files = {}
    pattern_run = re.compile(r"RUN\.(\d+)\.")

    try:
        for root, _, files in os.walk(cartella_completa):
            # Ordina i file 
            files.sort()
            for file in files:
                if file.endswith('.rtraw') and "ds-"+ TriggerType in file:
                    match = pattern_run.search(file)
                    if match:
                        numero_run = match.group(1)
                        percorso_completo = os.path.join(root, file)

                        # Aggiungo il file alla lista della RUN corrispondente
                        if numero_run not in run_files:
                            run_files[numero_run] = []
                        run_files[numero_run].append(percorso_completo)

        # Salvo tutto nella cartella 'list'
        for numero_run, files in run_files.items():
            if (TriggerType=="2") : 
                nome_file_output = os.path.join(cartella_list, f"rawfile_RUN{numero_run}.list")
            elif (TriggerType == "3") :
                nome_file_output = os.path.join(cartella_list, f"rawfile_RUN{numero_run}_MMtrigger.list")
            else :
                nome_file_output = os.path.join(cartella_list, f"rawfile_RUN{numero_run}_UNKNOWNtrigger.list")                
            with open(nome_file_output, 'w') as output_file:
                for percorso in files:
                    output_file.write(percorso + '\n')
            print(f"Creato file: {nome_file_output} con {len(files)} file")

    except Exception as e:
        print(f"Errore durante la creazione delle liste: {e}")

def main():
    parser = argparse.ArgumentParser(description="Crea file .list per RUN dai file .rtraw.")
    parser.add_argument("--cartella_finale", help="Nome della sottocartella finale (es. '1208').")
    parser.add_argument("--TriggerType",default="2",help="Nome ds-x dei file che vuoi leggere")    

    args = parser.parse_args()
    crea_liste_per_run(args.cartella_finale,args.TriggerType)

if __name__ == "__main__":
    main()

